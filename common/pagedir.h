/* 
 * pagedir - utility functions for downloading, saving, and loading web pages
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include "../libcs50/webpage.h"

/**************** pagedir_init ****************/
/* Initializes and validates that a directory is apt to write downloaded webpages to.
 *
 * Caller provides: 
 *  pageDirectory string representing the path of the directory
 *
 * We return:
 *  true if directory can be written downloaded webpages to, false otherwise
 *
 * IMPORTANT:
 *  program crashes cleanly if pageDirectory is NULL
 *  initialization/validation is performed by creating/checking the existence of a writable '.crawler' file inside pageDirectory
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Writes information about a page to a page file.
 *
 * Caller provides:
 *  page  webpage_t struct pointer that has information about a webpage including the url, depth, and html
 *  pageDirectory string representing the path of the directory where this page file is located
 *  docID the unique document ID of the webpage that identifies its page file
 *
 * IMPORTANT:
 *  program crashes cleanly if:
 *    any pointer argument is NULL
 *    file to write page information to cannot be opened
 * 
 * Limitations:
 *  docID should be an integer of less than 6 digits; else, it gets cut off at the 5-digit mark, leading to unexpected behavior
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);


/**************** pagedir_validate ****************/
/* Validates that a given directory is crawler-produced
 *
 * Caller provides:
 *  pageDirectory string representing the path of potentially crawler-produced directory
 *
 * We return:
 *  true if directory is crawler-produced, false otherwise
 *
 * IMPORTANT:
 *  program crashes cleanly if pageDirectory is NULL
 * 
 * Limitations:
 *  a directory that is not crawler-produced might still give a false positive (if it just so happens that it has readable files
 *  named '.crawler' and '1')
 */
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_load ****************/
/* Loads all page information from a file into a webpage_t struct
 *
 * Caller provides:
 *  pageDirectory string representing the path of the directory where this page file is located
 *  docID the unique document ID of the page that identifies its page file
 *
 * We return:
 *  pointer to webpage_t struct containing all page information, or
 *  NULL if page file does not exist or is not readable in directory, or if there was an error constructing webpage_t struct
 *
 * We assume:
 *  all files in pageDirectory are crawler-produced
 *
 * IMPORTANT:
 *  program crashes cleanly if:
 *    any pointer argument is NULL
 *    file containing page information cannot be opened
 * 
 * Limitations:
 *  docID should be an integer of less than 6 digits; else, it gets cut off at the 5-digit mark, leading to unexpected behavior
 */
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

/**************** pagedir_open ****************/
/* Opens a file identified by docID in pageDirectory in a given mode
 *
 * Caller provides:
 *  pageDirectory string pathname of directory where this page file is located
 *  docID         the unique document ID of the page that identifies its page file
 *  mode          mode in which this file should be opened
 *
 * We return:
 *  file pointer of page file
 *
 * We assume:
 *  all files in pageDirectory are crawler-produced
 *
 * IMPORTANT:
 *  program crashes cleanly if:
 *    pageDirectory is NULL
 *    file containing page information cannot be opened in given mode
 * 
 * Limitations:
 *  docID should be an integer of less than 6 digits; else, it gets cut off at the 5-digit mark, leading to unexpected behavior
 */
FILE* pagedir_open(const char* pageDirectory, const int docID, char* mode);
