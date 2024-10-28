/* 
 * index - data structure and utility functions for representing an index in memory
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

/***********************************************************************/
/* index_t: struct to represent an index that maps from word to (docID, count) pairs
 */
typedef struct index index_t;

/**************** index_new ****************/
/* Allocate and initialize a new index_t structure.
 *
 * Caller provides:
 *   numSlots  integer number of slots used for internal hashtable
 *
 * We return:
 *   pointer to new index_t struct, or NULL on any error
 *
 * Caller is responsible for:
 *   later calling index_delete with returned pointer
 *
 * IMPORTANT:
 *   program crashes cleanly if memory could not be allocated for index
 */
index_t* index_new(int numSlots);

/**************** index_add ****************/
/* Increment count of word's counter corresponding to document referenced by its ID
 * 
 * Caller provides:
 *   index  pointer to valid index_t struct
 *   word   word string
 *   docID  integer ID of webpage document (must be > 0)
 *
 * We guarantee:
 *   the counter's value will be >= 1, on successful return.
 *
 * We do:
 *  nothing if index is NULL, word is NULL, or docID < 1
 *  if the word does not yet exist, create a counter set for it, create a counter for the docID and initialize it to 1
 *  if the word exists but docID counter doesn't, create a counter for the docID and initialize it to 1
 *  if the word and docID counter exist, increment it by 1.
 *
 * Notes:
 *   The word string is copied for use by the index; that is, the module
 *   is responsible for allocating memory for a copy of the word string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its word string after this call.  
 */
void index_add(index_t* index, char* word, int docID);

/**************** index_set ****************/
/* Set count of word's counter corresponding to document referenced by its ID
 * 
 * Caller provides:
 *   index  pointer to valid index_t struct
 *   word   word string
 *   docID  integer ID of webpage document (must be > 0)
 *   count  integer to be set as count of word's counter (must be >= 0)
 *
 * We do:
 *  nothing if index is NULL, word is NULL, docID < 1, or count < 0
 *  if the word does not yet exist, create a counter set for it, create a counter for the docID and initialize it to count
 *  if the word exists but docID counter doesn't, create a counter for the docID and initialize it to count
 *  if the word and docID counter exist, set it to count
 *
 * Notes:
 *   The word string is copied for use by the index; that is, the module
 *   is responsible for allocating memory for a copy of the word string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its word string after this call.  
 */
void index_set(index_t* index, char* word, int docID, int count);

/**************** index_save ****************/
/* Saves all index information to a file
 * 
 * Caller provides:
 *   index  pointer to valid index_t struct
 *   indexFilename  filename of file we should write to
 *
 * We do:
 *   nothing, if index is NULL, indexFilename is NULL, or indexFile is not writable
 *   otherwise, we save information to indexFile in the following format: 'word docID counter [docID counter]...'
 */
void index_save(index_t* index, char* indexFilename);

/**************** index_load ****************/
/* Loads all information from an indexer-produced file to an index in memory
 * 
 * Caller provides:
 *   indexFilename  pathname of indexer-produced file
 * 
 * We return:
 *   pointer to loaded index_t struct, or
 *   NULL on any error (indexFilename is NULL or indexFile is not readable)
 *
 * IMPORTANT:
 *   program crashes cleanly if memory could not be allocated for index
 */
index_t* index_load(char* indexFilename);

/**************** index_delete ****************/
/* Free all memory allocated for the index
 * 
 * Caller provides:
 *   index  pointer to valid index_t struct
 *
 * We do:
 *   nothing, if index is NULL
 *   otherwise, free memory for index and all its contents
 */
void index_delete(index_t* index);
