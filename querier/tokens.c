/* 
 * tokens - data structure and utility functions for representing tokens of a query
 *
 * By Rodrigo Vega Ayllon - November 2024
 */

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/mem.h"
#include "../common/word.h"
#include "tokens.h"

/* tokens_t: structure to represent the tokens of a query and their relevant information
 * The innards should not be visible to users of the index module.
 */
typedef struct tokens {
  char** tokens;
  int length;
} tokens_t;

/* *********************************************************************** */
/* Public methods */

/**************** tokens_getLength ****************/
/* see tokens.h for documentation */
int tokens_getLength(tokens_t* tokens)
{
  if (tokens == NULL) {
    return -1;
  }

  return tokens->length;
}

/**************** tokens_new ****************/
/* see tokens.h for documentation */
tokens_t* tokens_new(int length)
{
  if (length <= 0) {
    return NULL;
  }

  // Initialize tokens struct
  tokens_t* tokens = malloc(sizeof(tokens));
  mem_assert(tokens, "could not allocate memory for tokens struct");

  // Initialize tokens array
  tokens->tokens = malloc(length * sizeof(char*));
  mem_assert(tokens, "could not allocate memory for tokens array");

  // Initialize tokens array length
  tokens->length = length;

  return tokens;
}

/**************** tokens_get ****************/
/* see tokens.h for documentation */
char* tokens_get(tokens_t* tokens, int index)
{
  if (tokens == NULL || index < 0 || index >= tokens_getLength(tokens)) {
    return NULL;
  }

  return tokens->tokens[index];
}

/**************** tokens_set ****************/
/* see tokens.h for documentation */
void tokens_set(tokens_t* tokens, int index, char* token)
{
  if (tokens == NULL || index < 0 || index >= tokens_getLength(tokens) || token == NULL) {
    return;
  }

  tokens->tokens[index] = token;
}

/**************** tokens_tokenize ****************/
/* see tokens.h for documentation */
tokens_t* tokens_tokenize(char* query)
{
  if (query == NULL) {
    return NULL;
  }

  // Iterate over query characters to get word count
  int wordCount = 0;
  char c = '\0';
  for (int i = 0; (c = query[i]) != '\0'; i++) {
    // Ignore if not alphabetic character
    if (isalpha(c) == 0) {
      continue;
    }

    // While we are 'in a word', keep going until we reach the end, then add one to counter
    while (isalpha(c) != 0) {
      c = query[++i];
    }
    wordCount++;

    if (c == '\0') {
      break;
    }
  }

  if (wordCount == 0) {
    return NULL;
  }

  // Iterate over characters in query to populate tokens array
  tokens_t* tokens = tokens_new(wordCount);
  int tokensPosition = 0;
  c = '\0';
  for (int i = 0; (c = query[i]) != '\0'; i++) {
    // Ignore if not alphabetic characer
    if (isalpha(c) == 0) {
      continue;
    }

    // While we are 'in a word', count each letter
    int letterCount = 0;
    while (isalpha(c) != 0) {
      letterCount++;
      c = query[++i];
    } 

    // Allocate memory for token according to letter count
    char* token = malloc((letterCount + 1) * sizeof(char));
    mem_assert(token, "could not allocate memory for token");

    // Go back to beginning of word, iterate over each character, populate token
    int tokenPosition = 0;
    int j = i - letterCount;
    while (j < i) {
      token[tokenPosition] = query[j];
      tokenPosition++;
      j++;
    }
    token[tokenPosition] = '\0'; // don't forget null char at the end

    // Make token lowercase
    word_normalizeWord(token);

    // Add token to tokens array
    tokens_set(tokens, tokensPosition, token);

    tokensPosition++;

    if (c == '\0') {
      break;
    }
  }

  return tokens;
}

/**************** tokens_delete ****************/
/* see tokens.h for documentation */
void tokens_delete(tokens_t* tokens)
{
  if (tokens == NULL) {
    return;
  }

  int tokensLength = tokens_getLength(tokens);

  // Iterate over each malloc'd token and free it
  for (int i = 0; i < tokensLength; i++) {
    free(tokens_get(tokens, i));
  }

  // Free tokens array and struct
  free(tokens->tokens);
  free(tokens);
}
