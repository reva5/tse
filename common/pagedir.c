/* 
 * pagedir - utility functions for outputting a page to the appropriate text file
 *           See pagedir.h for usage.
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include <string.h>
#include "pagedir.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"

/**************** pagedir_init ****************/
/* see pagedir.h for documentation */
bool pagedir_init(const char* pageDirectory)
{
  if (pageDirectory == NULL) {
    fprintf(stderr, "pageDirectory string is NULL\n");
    exit(1);
  }

  int dotfilePathLength = strlen("/.crawler") + strlen(pageDirectory) + 1;
  char dotfilePath[dotfilePathLength];
  snprintf(dotfilePath, dotfilePathLength, "%s/.crawler", pageDirectory);
  FILE* dotfile = fopen(dotfilePath, "w");
 
  if (dotfile == NULL) { // if failed opening file
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

  if (pageDirectory == NULL) {
    fprintf(stderr, "pageDirectory string is NULL\n");
    exit(1);
  }

  char docIDString[6] = "";
  snprintf(docIDString, 6, "%d", docID);

  int pagePathLength = strlen(pageDirectory) + strlen("/") + strlen(docIDString) + 1;
  char pagePath[pagePathLength];
  snprintf(pagePath, pagePathLength, "%s/%s", pageDirectory, docIDString);
  FILE* pageFile = fopen(pagePath, "w");
  mem_assert(pageFile, "failed opening file pageFile for writing\n");

  fprintf(pageFile, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
  fclose(pageFile);
}
