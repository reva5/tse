/* 
 * indexer - standalone program that reads files produced by crawler, builds an index, and writes that index to a file
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include <string.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/webpage.h"

static void parseArgs(char* pageDirectory, char* indexFilename);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, int docID);

/**************** main ****************/
/* Entry point of the program. Validate correct usage, then call parseArgs and save pageDirectory index to indexFilename.
 *
 * Caller provides: 
 *  argc  number of command-line arguments
 *  argv  string array of the command-line arguments
 *
 * We return:
 *  0 on success, 1 on failure
 *
 * Usage:
 *  ./indexer pageDirectory indexFilename
 *    pageDirectory - pathname of directory produced by crawler
 *    indexFilename - pathname of a file into which the index should be written
 */
int main(const int argc, char* argv[])
{
  // Ensure correct number of arguments
  if (argc != 3) {
    fprintf(stderr, "usage: ./indexer pageDirectory indexFilename\n\tpageDirectory - pathname of directory ");
    fprintf(stderr, "produced by crawler\n\tindexFilename - pathname of a file into which the index should ");
    fprintf(stderr, "be written\n");
    exit(1);
  }

  // Parse command-line arguments
  parseArgs(argv[1], argv[2]);

  // Build index from pageDirectory
  index_t* index = indexBuild(argv[1]);

  // Save index to indexFilename
  index_save(index, argv[2]);

  // Delete index (in memory)
  index_delete(index);

  exit(0);
}

/**************** parseArgs ****************/
/* Parse command-line arguments so they meet minimum functionality requirements.
 *
 * Caller provides: 
 *  pageDirectory pathname of directory produced by crawler
 *  indexFilename pathname of file into which index is written 
 *
 * We only return on success, exit non-zero otherwise
 *
 * We assume:
 *  indexFilename gets created (if needed) or overwritten (if it already exists)
 */
static void parseArgs(char* pageDirectory, char* indexFilename)
{ 
  // Ensure pageDirectory is crawler-produced
  if (pagedir_validate(pageDirectory) == false) {
    fprintf(stderr, "pageDirectory %s is not crawler-produced\n", pageDirectory);
    exit(1);
  }

  // Check if indexFilename can be created/opened in 'write' mode
  FILE* indexFile = NULL;
  if ((indexFile = fopen(indexFilename, "w")) == NULL) {
    fprintf(stderr, "failed opening writable index file %s\n", indexFilename);
    exit(1);
  }

  fclose(indexFile);
}

/**************** indexBuild ****************/
/* Build an in-memory index from webpage files in pageDirectory.
 *
 * Caller provides: 
 *  pageDirectory pathname of directory produced by crawler
 *
 * We return:
 *  pointer to index_t struct built from page files in pageDirectory
 */
static index_t* indexBuild(char* pageDirectory)
{
  // Initialize index
  index_t* index = index_new(600); // # of slots based on amount of data we expect to process

  // Load webpage from each page file and scan it for words
  webpage_t* page = NULL;
  for (int docID = 1; (page = pagedir_load(pageDirectory, docID)) != NULL; docID++) {
    indexPage(index, page, docID);
    webpage_delete(page);
  }

  return index;
}

/**************** indexPage ****************/
/* Scan webpage document to add its words to the index.
 *
 * Caller provides: 
 *  index pointer to index_t struct
 *  page pointer to webpage_t struct holding information from a webpage document
 *  docID integer ID of webpage document
 */
static void indexPage(index_t* index, webpage_t* page, int docID)
{
  // Initialize variables
  char* word = "";
  int pos = 0;

  // Step through each word in webpage
  while ((word = webpage_getNextWord(page, &pos)) != NULL) {
    // Skip trivial words (less than length 3)
    if (strlen(word) < 3) {
      free(word);
      continue;
    }

    // Normalize word (convert it to lower case)
    word_normalizeWord(word);

    // Increment the count of occurrences of word in document ID
    index_add(index, word, docID);

    free(word);
  }
}
