/* 
 * tokens - data structure and utility functions for representing tokens of a query
 *
 * By Rodrigo Vega Ayllon - November 2024
 */

/* tokens_t: structure to represent the tokens of a query and their relevant information */
typedef struct tokens tokens_t;

/*** Getters ***/
int tokens_getLength(tokens_t* tokens);

/**************** tokens_new ****************/
/* Allocate and initialize a new tokens_t structure.
 *
 * Caller provides:
 *   length  integer number of tokens
 *
 * We return:
 *   pointer to new tokens_t struct, or NULL if length <= 0
 *
 * Caller is responsible for:
 *   later calling tokens_delete with returned pointer
 *
 * IMPORTANT:
 *   program crashes cleanly if memory could not be allocated
 */
tokens_t* tokens_new(int length);

/**************** tokens_get ****************/
/* Get reference to a token in tokens.
 *
 * Caller provides:
 *   tokens pointer to tokens_t struct
 *   index  integer index of tokens array
 *
 * We return:
 *   NULL if tokens is NULL or index is out of bounds
 *   otherwise, return token string
 */
char* tokens_get(tokens_t* tokens, int index);

/**************** tokens_set ****************/
/* Set token at a given index.
 * 
 * Caller provides:
 *   tokens pointer to tokens_t struct
 *   index  integer index of tokens array
 *   token  string query token
 *
 * We do:
 *  nothing if tokens is NULL, index is out of bounds, or token is NULL
 *  else, set token at given index to be the given token
 *
 * We assume:
 *  token is malloc'd
 */
void tokens_set(tokens_t* tokens, int index, char* token);

/**************** tokens_tokenize ****************/
/* Divide a given query string into tokens.
 * 
 * Caller provides:
 *   query  string query
 *
 * We return:
 *   NULL if query is NULL or has no words in it
 *   otherwise, pointer to tokens_t struct with fields populated accordingly (tokens are malloc'd!)
 */
tokens_t* tokens_tokenize(char* query);

/**************** tokens_delete ****************/
/* Free all memory allocated for tokens.
 * 
 * Caller provides:
 *   tokens pointer to tokens_t struct
 *
 * We do:
 *   nothing if tokens is NULL
 *   otherwise, we free memory allocated for each malloc'd token, the tokens array, and the tokens_t struct
 */
void tokens_delete(tokens_t* tokens);
