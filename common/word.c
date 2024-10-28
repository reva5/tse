/* 
 * word - module providing a function to normalize a word
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include <ctype.h>
#include "word.h"

/**************** word_normalizeWord ****************/
/* see word.h for documentation */
void word_normalizeWord(char* word)
{
  int i = 0;

  // Loop through all word characters
  for (i = 0; word[i]; i++) {
    // Convert character to lowercase
    word[i] = tolower(word[i]);
  }
  
  // Don't forget null character at the end
  word[i] = '\0'; 
}
