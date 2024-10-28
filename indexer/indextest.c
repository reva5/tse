/* 
 * indextest - program that loads an index file produced by the indexer and saves it to another file
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include <stdlib.h>
#include <stdio.h>
#include "../common/index.h"

/**************** main ****************/
/* Entry point of the program. Validate correct usage, parse arguments, then load and save index.
 *
 * Caller provides: 
 *  argc  number of command-line arguments
 *  argv  string array of the command-line arguments
 *
 * We return:
 *  0 on success, 1 on failure
 *
 * Usage:
 *  ./indextest oldIndexFilename newIndexFilename
 *    oldIndexFilename - pathname of a file produced by the indexer
 *    newIndexFilename - pathname of a file into which the index should be written
 */
int main(const int argc, char* argv[])
{
  // Ensure correct number of command-line arguments
  if (argc != 3) {
    fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename\n\toldIndexFilename - pathname of a ");
    fprintf(stderr, "file produced by the indexer\n\tnewIndexFilename - pathname of a file into which the index ");
    fprintf(stderr, "should be written\n");
    exit(1);
  }

  // Ensure oldIndexFilename is readable
  FILE* indexFile = fopen(argv[1], "r");  
  if (indexFile == NULL) {
    fprintf(stderr, "indexFile %s is not readable\n", argv[1]);
    exit(1);
  }
  fclose(indexFile);

  // Ensure newIndexFilename is writable
  indexFile = fopen(argv[2], "w");
  if (indexFile == NULL) {
    fprintf(stderr, "indexFile %s is not writable\n", argv[2]);
    exit(1);
  }
  fclose(indexFile);

  // Load index from oldIndexFilename
  index_t* index = index_load(argv[1]);

  // Save index to newIndexFilename
  index_save(index, argv[2]);

  index_delete(index);

  exit(0);
}
