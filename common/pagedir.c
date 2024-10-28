/* 
 * pagedir - utility functions for outputting a page to the appropriate text file
 *           See pagedir.h for usage.
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include <string.h>
#include <unistd.h>
#include "pagedir.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/* *********************************************************************** */
/* Private function prototypes */

static FILE* openPageFile(const char* pageDirectory, const int docID, char* mode);

/* *********************************************************************** */
/* Public methods */

/**************** pagedir_init ****************/
/* see pagedir.h for documentation */
bool pagedir_init(const char* pageDirectory)
{
  if (pageDirectory == NULL) {
    fprintf(stderr, "pageDirectory string is NULL\n");
    exit(1);
  }

  // Construct '.crawler' file path and create/overwrite it
  int dotfilePathLength = strlen("/.crawler") + strlen(pageDirectory) + 1;
  char dotfilePath[dotfilePathLength];
  snprintf(dotfilePath, dotfilePathLength, "%s/.crawler", pageDirectory);
  FILE* dotfile = fopen(dotfilePath, "w");
 
  if (dotfile == NULL) { // if failed creating/overwriting it
    return false;
  } else {
    fclose(dotfile);
    return true;
  }
}

/**************** pagedir_save ****************/
/* see pagedir.h for documentation */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
  if (page == NULL) {
    fprintf(stderr, "page webpage_t pointer is NULL\n");
    exit(1);
  }

  FILE* pageFile = openPageFile(pageDirectory, docID, "w");
  fprintf(pageFile, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
  fclose(pageFile);
}

/**************** pagedir_validate ****************/
/* see pagedir.h for documentation */
bool pagedir_validate(const char* pageDirectory)
{
  if (pageDirectory == NULL) {
    fprintf(stderr, "pageDirectory string is NULL\n");
    exit(1);
  }

  // Construct '.crawler' file path
  int dotfilePathLength = strlen(pageDirectory) + strlen("/.crawler") + 1;
  char dotfilePath[dotfilePathLength];
  snprintf(dotfilePath, dotfilePathLength, "%s/.crawler", pageDirectory);

  // Construct '1' file path
  int pagePathLength = strlen(pageDirectory) + 3; // add 3 to account for '/', '1' and NULL character
  char pagePath[pagePathLength];
  snprintf(pagePath, pagePathLength, "%s/1", pageDirectory);

  // Check for existence and readibility of both '.crawler' and '1' files
  return (access(pagePath, R_OK) == 0 && access(dotfilePath, R_OK) == 0);
}

/**************** pagedir_load ****************/
/* see pagedir.h for documentation */
webpage_t* pagedir_load(const char* pageDirectory, const int docID)
{
  // Convert docID int to string
  char docIDString[6] = "";
  snprintf(docIDString, 6, "%d", docID);

  // Construct page file path
  int pagePathLength = strlen(pageDirectory) + strlen("/") + strlen(docIDString) + 1;
  char pagePath[pagePathLength];
  snprintf(pagePath, pagePathLength, "%s/%s", pageDirectory, docIDString);

  // Check if page file doesn't exist or is not readable
  if (access(pagePath, R_OK) != 0) {
    return NULL;
  }

  // Open page file
  FILE* pageFile = openPageFile(pageDirectory, docID, "r");

  // Read lines of page file and assign them to their corresponding variables
  char* URL = file_readLine(pageFile);
  char* depthString = file_readLine(pageFile);
  char* HTML = file_readFile(pageFile);

  // Convert depth from string to int
  int depth = strtol(depthString, NULL, 10);

  // Clean up
  free(depthString);
  fclose(pageFile);

  // Return pointer to webpage_t struct with page information
  return webpage_new(URL, depth, HTML);
}

/***********************************************************************
 * INTERNAL FUNCTIONS
 ***********************************************************************/

/**************** openPageFile ****************/
/* return a file pointer to a page file referenced by its document ID 
 * limitations: docID must have <= 5 digits
 */
static FILE* openPageFile(const char* pageDirectory, const int docID, char* mode)
{
  if (pageDirectory == NULL) {
    fprintf(stderr, "pageDirectory string is NULL\n");
    exit(1);
  }

  // Convert docID int to string
  char docIDString[6] = "";
  snprintf(docIDString, 6, "%d", docID);

  // Construct page file path
  int pagePathLength = strlen(pageDirectory) + strlen("/") + strlen(docIDString) + 1;
  char pagePath[pagePathLength];
  snprintf(pagePath, pagePathLength, "%s/%s", pageDirectory, docIDString);

  // Open page file
  FILE* pageFile = fopen(pagePath, mode);
  mem_assert(pageFile, "failed opening file pageFile");

  // Return file pointer
  return pageFile;
}
