/* 
 * index - data structure and utility functions for representing an index in memory
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "index.h"

/* index_t: structure to represent an index that maps from words to (docID, count) pairs
 * The innards should not be visible to users of the index module.
 */
typedef struct index {
  hashtable_t* words;
} index_t;

/* *********************************************************************** */
/* Private function prototypes */

static void wordprint(void* arg, const char* key, void* item);
static void pairprint(void* arg, const int key, const int count);

/* *********************************************************************** */
/* Public methods */

/**************** index_new ****************/
/* see index.h for documentation */
index_t* index_new(int numSlots)
{
  // Allocate memory for index_t struct
  index_t* index = mem_assert(malloc(sizeof(index_t)), "failed allocating memory for index");

  // Initialize words hashtable
  index->words = hashtable_new(numSlots);

  return index;
}

/**************** index_add ****************/
/* see index.h for documentation */
void index_add(index_t* index, char* word, int docID)
{
  if (index == NULL || docID < 1) {
    return;
  }

  // Get counter set of word
  counters_t* IDToOccurrences = hashtable_find(index->words, word);

  // If no counter set, create one and insert it
  if (IDToOccurrences == NULL) {
    IDToOccurrences = counters_new();
    hashtable_insert(index->words, word, IDToOccurrences);
  }

  // Add one to counter corresponding to docID
  counters_add(IDToOccurrences, docID);
}

/**************** index_set ****************/
/* see index.h for documentation */
void index_set(index_t* index, char* word, int docID, int count)
{
  if (index == NULL || docID < 1) {
    return;
  }

  // Get counter set of word
  counters_t* IDToOccurrences = hashtable_find(index->words, word);

  // If no counter set, create one and insert it
  if (IDToOccurrences == NULL) {
    IDToOccurrences = counters_new();
    hashtable_insert(index->words, word, IDToOccurrences);
  }

  // Set counter corresponding to docID to count
  counters_set(IDToOccurrences, docID, count);
}

/**************** index_save ****************/
/* see index.h for documentation */
void index_save(index_t* index, char* indexFilename)
{
  if (index == NULL || indexFilename == NULL) {
    return;
  }

  // Try to open indexFile and check if it was successful
  FILE* indexFile = fopen(indexFilename, "w");
  if (indexFile == NULL) {
    return;
  }

  // Print to indexFile each word in index in the format 'word docID count [docID count]...'
  hashtable_iterate(index->words, indexFile, wordprint);

  fclose(indexFile);
}

/**************** index_load ****************/
/* see index.h for documentation */
index_t* index_load(char* indexFilename)
{
  if (indexFilename == NULL) {
    return NULL;
  }

  // Try to open indexFile and check if it was successful
  FILE* indexFile = fopen(indexFilename, "r");
  if (indexFile == NULL) {
    return NULL;
  }

  // Get number of words in indexFile and assign number of slots based on that
  int numWords = file_numLines(indexFile);
  index_t* index = index_new(numWords / 10);

  // Read word at the beginning of each line
  char* word = "";
  while ((word = file_readWord(indexFile)) != NULL) {
    int docID = 0;
    int count = 0;

    // Pull off one (docID, count) pair at a time
    while ((fscanf(indexFile, "%d %d", &docID, &count)) == 2) {
      index_set(index, word, docID, count); // set count corresponding to docID of word
    }

    free(word);
  }

  fclose(indexFile);

  return index;
}

/**************** index_delete ****************/
/* see index.h for documentation */
void index_delete(index_t* index)
{
  if (index == NULL) {
    return;
  }

  // Delete words hashtable
  hashtable_delete(index->words, (void (*)(void*)) counters_delete);

  // Free memory for index_t struct
  free(index);
}

/***********************************************************************
 * INTERNAL FUNCTIONS
 ***********************************************************************/

/* ****************** wordprint ***************************** */
/* print a word in index_t struct to a file in the format 'word docID count [docID count]...'
 */
static void wordprint(void* arg, const char* key, void* item)
{
  // Cast void pointer to file pointer
  FILE* indexFile = (FILE*) arg;

  // Print 'word ' to file
  fprintf(indexFile, "%s ", key);

  // Print all (docID, count) pairs in the format 'docID count '
  counters_t* IDToOccurrences = (counters_t*) item;
  counters_iterate(IDToOccurrences, indexFile, pairprint);

  // Print newline to file
  fprintf(indexFile, "\n");
}

/* ****************** pairprint ***************************** */
/* print a (docID, count) pair to a file in the format 'docID count'
 */
static void pairprint(void* arg, const int key, const int count)
{
  // Cast void pointer to file pointer
  FILE* indexFile = (FILE*) arg;

  // Print (docID, count) pair to file in the format 'docID count '
  fprintf(indexFile, "%d %d ", key, count);
}
