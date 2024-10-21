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
 *  it is assumed that pageDirectory does not contain any files whose name is an integer (i.e. 1, 2,...)
 * 
 * Limitations:
 *  docID should be an integer of less than 6 digits; else, it gets cut off at the 5-digit mark, leading to unexpected behavior
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
