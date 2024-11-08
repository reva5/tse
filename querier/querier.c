/* 
 * querier - standalone program that reads an 'indexer' index file and 'crawler' page files, and answers search queries via stdin
 *
 * By Rodrigo Vega Ayllon - November 2024
 */

#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "tokens.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../common/index.c"
#include "../common/index.h"
#include "../common/pagedir.h"

int fileno(FILE* stream);

static void parseArgs(char* pageDirectory, char* indexFilename);
static bool parseQuery(char* query);
static bool parseTokens(tokens_t* tokens);
static counters_t* processQuery(tokens_t* tokens, index_t* index);
static void rankPages(counters_t* pages, char* pageDirectory);
static int respondQuery(index_t* index, char* pageDirectory);

static void prompt(void);
static void intersectWords(counters_t** wordACounters, counters_t* wordBCounters);
static void unionWords(counters_t* wordACounters, counters_t* wordBCounters);

static void countermin(void* arg, const int key, const int count);
static void countersum(void* arg, const int key, const int count);
static void countermax(void* arg, const int key, const int count);
static void countercount(void* arg, const int key, const int count);

/**************** main ****************/
/* Entry point of the program. Validate correct usage of and parse command-line arguments, load index from
 * indexFilename, then receive, process, and satisfy each query from stdin
 *
 * Caller provides: 
 *  argc  number of command-line arguments
 *  argv  string array of the command-line arguments
 *
 * We return:
 *  0 on success, 1 on failure
 *
 * Usage:
 *  ./querier pageDirectory indexFilename
 *    pageDirectory - pathname of directory produced by the Crawler
 *    indexFilename - pathname of a file produced by the Indexer
 */
int main(const int argc, char* argv[])
{
  // Ensure correct number of arguments
  if (argc != 3) {
    fprintf(stderr, "usage: ./querier pageDirectory indexFilename\n\tpageDirectory - pathname of directory ");
    fprintf(stderr, "produced by the Crawler\n\tindexFilename - pathname of a file produced by the Indexer");
    exit(1);
  }

  // Parse command-line arguments
  parseArgs(argv[1], argv[2]);

  // Load index from indexFilename
  index_t* index = index_load(argv[2]);

  // Receive queries until we receive EOF as input
  int responseStatus = 0;
  while (responseStatus != 2) {
    responseStatus = respondQuery(index, argv[1]);
  }

  index_delete(index);
}

/**************** parseArgs ****************/
/* Parse command-line arguments so they meet minimum functionality requirements.
 *
 * Caller provides: 
 *  pageDirectory pathname of directory produced by the Crawler
 *  indexFilename pathname of file produced by the Indexer
 *
 * We only return on success, exit non-zero otherwise
 */
static void parseArgs(char* pageDirectory, char* indexFilename)
{
  // Ensure pageDirectory is crawler-produced
  if (pagedir_validate(pageDirectory) == false) {
    fprintf(stderr, "pageDirectory %s is not crawler-produced\n", pageDirectory);
    exit(1);
  }

  // Check if indexFilename can be opened in 'read' mode
  FILE* indexFile = NULL;
  if ((indexFile = fopen(indexFilename, "r")) == NULL) {
    fprintf(stderr, "failed opening readable index file %s\n", indexFilename);
    exit(1);
  }

  fclose(indexFile);
}


/**************** parseQuery ****************/
/* Parse query so that all its characters are either alphabetic or whitespace.
 *
 * Caller provides: 
 *  query user-input string from stdin
 *
 * We return:
 *  true if all characters are either alphabetic or whitespace, false otherwise
 */
static bool parseQuery(char* query)
{
  // Iterate over query characters
  char c = '\0';
  for (int i = 0; query[i]; i++) {
    // Ensure character is either alphabetic or whitespace
    c = query[i];
    if (isalpha(c) == false && isspace(c) == false) {
      fprintf(stderr, "Error: bad character '%c' in query.\n", c);
      return false;
    }
  }

  return true;
}

/**************** parseTokens ****************/
/* Parse query tokens so operators in it are not first, last, or adjacent.
 *
 * Caller provides: 
 *  tokens  pointer to tokens_t struct built from query
 *
 * We return:
 *  false if any operator is first, last, or adjacent with any other operator; true otherwise
 */
static bool parseTokens(tokens_t* tokens)
{
  int tokensLength = tokens_getLength(tokens);

  // Ensure first token is not an operator
  char* firstToken = tokens_get(tokens, 0);
  if (strcmp(firstToken, "and") == 0 || strcmp(firstToken, "or") == 0) {
    fprintf(stderr, "Error: '%s' cannot be first.\n", firstToken);
    return false;
  }

  // Ensure last token is not an operator
  char* lastToken = tokens_get(tokens, tokensLength - 1);
  if (strcmp(lastToken, "and") == 0 || strcmp(lastToken, "or") == 0) {
    fprintf(stderr, "Error: '%s' cannot be last.\n", lastToken);
    return false;
  }

  // Ensure operators are not adjacent
  bool operator = false;
  for (int i = 1; i < tokensLength - 1; i++) {
    char* currToken = tokens_get(tokens, i);
    // Check if current token is operator
    if (strcmp(currToken, "and") == 0 || strcmp(currToken, "or") == 0) {
      // If last one was too, then error out
      if (operator == true) {
        char* prevToken = tokens_get(tokens, i - 1);
        fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent.\n", prevToken, currToken);
        return false;
      }
      // If last one was not, then proceed but remember this one is operator
      else {
        operator = true;
      }
    }
    else {
      operator = false;
    }
  }

  return true;
}

/**************** processQuery ****************/
/* Determine and score the pages that satisfy the query.
 *
 * Caller provides: 
 *  tokens  pointer to tokens_t struct built from query
 *  index pointer to index_t struct built from indexFilename
 *
 * We return:
 *  pointer to counters_t struct with pages as keys and scores as counts
 */
static counters_t* processQuery(tokens_t* tokens, index_t* index)
{
  int tokensLength = tokens_getLength(tokens);

  // Create page-score counters and iterate over query tokens
  counters_t* pages = counters_new();
  char* token = "";
  for (int i = 0; i < tokensLength; i++) {
    token = tokens_get(tokens, i);

    // Create counters that will temporarily hold the counters intersection of the 'andsequence'
    counters_t* temp = counters_new();

    // Since no "intersect identity", assign counters of first token to 'temp'
    counters_t* wordCounters = index_get(index, token);
    unionWords(temp, wordCounters);

    // Get next token
    token = tokens_get(tokens, ++i);

    // Iterate over 'andsequence'
    while (i < tokensLength && strcmp(token, "or") != 0) {
      // If token is 'and', ignore and proceed (since that is what we are already implicitly doing)
      if (strcmp(token, "and") == 0) {
        token = tokens_get(tokens, ++i);
        continue;
      }

      // Intersect temp with token counters (let temp hold the result)
      wordCounters = index_get(index, token);
      intersectWords(&temp, wordCounters);

      token = tokens_get(tokens, ++i);
    }

    // Union 'pages' counters with 'temp' counters (let pages hold the result)
    unionWords(pages, temp);

    // Clean up
    counters_delete(temp);
  }

  return pages;
}

/**************** rankPages ****************/
/* Rank pages according to their scores and print them to stdout.
 *
 * Caller provides: 
 *  pages         pointer to counters_t struct with pages as keys and scores as counts
 *  pageDirectory string pathname of crawler-produced directory
 */
static void rankPages(counters_t* pages, char* pageDirectory)
{
  int maxPageCount[2] = { 0, 0 }; 

  // Populate 'maxPageCount' with ID and score of highest-ranking page
  counters_iterate(pages, maxPageCount, countermax);

  // Set score of page to 0 in 'page' counters (so next time we get second-highest)
  counters_set(pages, maxPageCount[0], 0);

  // Repeat until all pages scores have been set to 0
  while (maxPageCount[1] != 0) {
    // Read URL of page
    FILE* pageFile = pagedir_open(pageDirectory, maxPageCount[0], "r");
    char* pageURL = file_readLine(pageFile);

    // Print result entry
    printf("score\t%d doc\t%d: %s\n", maxPageCount[1], maxPageCount[0], pageURL);

    // Clean up
    fclose(pageFile);
    free(pageURL);

    // Set up next iteration
    maxPageCount[0] = 0;
    maxPageCount[1] = 0;
    counters_iterate(pages, maxPageCount, countermax);
    counters_set(pages, maxPageCount[0], 0);
  }
}

/**************** respondQuery ****************/
/* Respond to a query with a list of matching pages.
 *
 * Caller provides: 
 *   index  pointer to a (populated) index_t struct 
 *
 * We return:
 *   0 if query was responded to successfully
 *   1 if query was invalid or an error occurred
 *   2 if query was 'EOF'
 */
static int respondQuery(index_t* index, char* pageDirectory)
{
  // Prompt for query and read it
  prompt();
  char* query = file_readLine(stdin);

  // Check if it's EOF
  if (query == NULL) {
    return 2; // terminate program
  }

  // Parse query
  if (parseQuery(query) == false) {
    free(query);
    return 1; // prompt for next query
  }

  // Tokenize query
  tokens_t* tokens = tokens_tokenize(query);

  // Ensure query is not empty
  if (tokens == NULL) {
    free(query);
    return 1;
  }

  // Parse tokens
  if (parseTokens(tokens) == false) {
    free(query);
    tokens_delete(tokens);
    return 1;
  }

  // Print clean query (from its tokenization)
  printf("Query: ");
  int i = 0;
  while (i < tokens_getLength(tokens) - 1) {
    printf("%s ", tokens_get(tokens, i));
    i++;
  }
  printf("%s\n", tokens_get(tokens, i));

  // Get all pages that match query
  counters_t* queryPages = processQuery(tokens, index);

  // Count number of pages
  int pageCount = 0;
  counters_iterate(queryPages, &pageCount, countercount);

  // Check if there are any pages or not
  if (pageCount == 0) {
    printf("No documents match.\n");
    printf("----------------------------------\n");
    free(query);
    tokens_delete(tokens);
    counters_delete(queryPages);
    return 0;
  }
  else {
    printf("Matches %d documents (ranked):\n", pageCount);
  }

  // Rank the pages by their score and print them to stdout
  rankPages(queryPages, pageDirectory);

  // Clean up
  free(query);
  tokens_delete(tokens);
  counters_delete(queryPages);
  printf("----------------------------------\n");

  return 0;
}

/**************** intersectWords ****************/
/* Intersect the (docID, # of occurrences) counters corresponding to words.
 *
 * Caller provides: 
 *  wordACounters pointer to pointer to counters_t struct corresponding to a word
 *  wordBCounters pointer to counters_t struct corresponding to another word
 */
static void intersectWords(counters_t** wordACounters, counters_t* wordBCounters)
{
  // Create variable to hold intersection
  counters_t* result = counters_new();

  // Iterate over 'wordBCounters'
  // If any counters in common (by key) with 'wordACounters', assign minimum (by count) to 'result'
  counters_t* bundle[2] = { *wordACounters, result };
  counters_iterate(wordBCounters, bundle, countermin);

  // Delete wordACounters pointer
  counters_delete(*wordACounters);

  // Set pointer to wordACounters to result
  *wordACounters = result;
}

/**************** unionWords ****************/
/* Union the (docID, # of occurrences) counters corresponding to words.
 *
 * Caller provides: 
 *  wordACounters pointer to counters_t struct corresponding to a word
 *  wordBCounters pointer to counters_t struct corresponding to another word
 */
static void unionWords(counters_t* wordACounters, counters_t* wordBCounters)
{
  // Iterate over 'wordBCounters'
  // Assign the sum of each counter + the corresponding counter in 'wordACounters' to 'wordBCounters'
  counters_iterate(wordBCounters, wordACounters, countersum);
}

/**************** prompt ****************/
/* Print a prompt if stdin is a tty (terminal). */
static void prompt(void)
{
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/**************** countermin ****************/
/* Helper counters_iterate 'itemfunc' function for intersectWords. */
static void countermin(void* arg, const int key, const int count)
{
  // Cast accordingly
  counters_t** bundle = (counters_t**) arg;
  counters_t* countersB = bundle[0];
  counters_t* result = bundle[1];

  // Check that key is in countersB
  int countB = 0;
  if ((countB = counters_get(countersB, key)) > 0) {
    // Set the key to the lower count in result
    int minCount = (count < countB) ? count : countB;
    counters_set(result, key, minCount);
  }
}

/**************** countersum ****************/
/* Helper counters_iterate 'itemfunc' function for unionWords. */
static void countersum(void* arg, const int key, const int count)
{
  counters_t* countersB = (counters_t*) arg;
  int sumCount = counters_get(countersB, key) + count;
  counters_set(countersB, key, sumCount);
}

/**************** countermax ****************/
/* Helper counters_iterate 'itemfunc' function for rankPages. */
static void countermax(void* arg, const int key, const int count)
{
  int* maxKeyCount = (int*) arg; 

  if (count > maxKeyCount[1]) {
    maxKeyCount[0] = key;
    maxKeyCount[1] = count;
  }
}

/**************** countercount ****************/
/* Helper counters_iterate 'itemfunc' function for respondQuery. */
static void countercount(void* arg, const int key, const int count)
{
  int* ccount = (int*) arg;
  *ccount += 1;
}
