/* 
 * crawler - standalone program that crawls the web and retrieves webpages starting from a "seed" URL
 *
 * By Rodrigo Vega Ayllon - October 2024
 */

#include "../common/pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** main ****************/
/* Entry point of the program. Validate correct usage, then simply call parseArgs and crawl.
 *
 * Caller provides: 
 *  argc  number of command-line arguments
 *  argv  string array of the command-line arguments
 *
 * We return:
 *  0 on success, 1 on failure
 *
 * Usage:
 *  ./crawler seedURL pageDirectory maxDepth
 *    seedURL - 'internal' directory, to be used as the initial URL
 *    pageDirectory - (existing) directory in which to write downloaded webpages
 *    maxDepth - integer in range [0..10] indicating the maximum crawl depth
 */
int main(const int argc, char* argv[])
{
  // Ensure correct number of arguments
  if (argc != 4) {
    fprintf(stderr, "usage: ./crawler seedURL pageDirectory maxDepth\n\tseedURL - 'internal' directory, to be used ");
    fprintf(stderr, "as the initial URL\n\tpageDirectory - (existing) directory in which to write download webpages");
    fprintf(stderr, "\n\tmaxDepth - integer in range [0..10] indicating the maximum crawl depth\n");
    exit(1);
  }

  // Convert maxDepth to integer
  char* end = NULL; // pointer to pointer to first character after numeric value
  int maxDepth = strtol(argv[3], &end, 10);
  if (*end != '\0') {
    fprintf(stderr, "maxDepth could not be converted to integer\n");
    exit(1);
  }

  // Parse command-line arguments
  parseArgs(argc, argv, &argv[1], &argv[2], &maxDepth);

  // Crawl the web
  crawl(argv[1], argv[2], maxDepth);

  exit(0);
}

/**************** parseArgs ****************/
/* Parse/modify command-line arguments so they meet minimum functionality requirements.
 *
 * Caller provides: 
 *  argc  number of command-line arguments
 *  argv  string array of the command-line arguments
 *  seedURL pointer to seed URL string
 *  pageDirectory pointer to page directory string (where pages will be saved)
 *  maxDepth pointer to integer indicating the maximum crawl depth
 *
 * We only return on success, exit non-zero otherwise
 *
 * We assume:
 *  maxDepth should be in the range [0..10]
 */
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{ 
  // Ensure seedURL is normalized
  if ((*seedURL = normalizeURL(*seedURL)) == NULL) {
    fprintf(stderr, "seedURL could not be normalized\n");
    exit(1); 
  }

  // Ensure seedURL is internal
  if (isInternalURL(*seedURL) == false) {
    fprintf(stderr, "seedURL %s is not internal\n", *seedURL);
    exit(1);
  }
  // Ensure pageDirectory is initialized
  if (pagedir_init(*pageDirectory) == false) {
    fprintf(stderr, "failed opening .crawler file in pageDirectory %s\n", *pageDirectory);
    exit(1);
  }

  // Ensure maxDepth is in range [0..10]
  if (*maxDepth < 0 || *maxDepth > 10) {
    fprintf(stderr, "maxDepth %d is not in range [0..10]\n", *maxDepth);
    exit(1);
  }
}

/**************** crawl ****************/
/* Crawl from seedURL to maxDepth and save pages in pageDirectory.
 *
 * Caller provides: 
 *  seedURL seed URL string
 *  pageDirectory page directory string (where pages will be saved)
 *  maxDepth integer indicating the maximum crawl depth
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
  // Initialize pagesSeen hashtable and insert seedURL
  hashtable_t* pagesSeen = mem_assert(hashtable_new(maxDepth + 1), "pagesSeen hashtable could not be initialized\n");
  if (hashtable_insert(pagesSeen, seedURL, "") == false) {
    fprintf(stderr, "could not insert seedURL %s to pagesSeen hashtable\n", seedURL);
    exit(1);
  }

  // Initialize pagesToCrawl bag and insert webpage with seedURL as URL, 0 as depth, and no HTML (yet)
  bag_t* pagesToCrawl = mem_assert(bag_new(), "pagesToCrawl bag could not be initialized\n");
  bag_insert(pagesToCrawl, webpage_new(seedURL, 0, NULL));
  
  // Crawl webpages to be crawled
  int docID = 1;
  webpage_t* webpage = NULL;
  while ((webpage = bag_extract(pagesToCrawl)) != NULL) {
    // Fetch webpage HTML
    if (webpage_fetch(webpage) == true) {
      printf("%d\tFetched: %s\n", webpage_getDepth(webpage), webpage_getURL(webpage));

      // Save webpage to pageDirectory
      pagedir_save(webpage, pageDirectory, docID);
      docID++;

      // Scan webpage if we are not at maxDepth yet
      if (webpage_getDepth(webpage) < maxDepth) {
        printf("%d\tScanning: %s\n", webpage_getDepth(webpage), webpage_getURL(webpage));
        pageScan(webpage, pagesToCrawl, pagesSeen); // retrieve relevant links from HTML and enqueue their processing
      }
    }
    
    webpage_delete(webpage);
  }

  // Delete pagesSeen hashtable and pagesToCrawl bag
  hashtable_delete(pagesSeen, NULL);
  bag_delete(pagesToCrawl, NULL);
}

/**************** pageScan ****************/
/* Given a webpage, scan the given page to extract any links (URLs), ignoring non-internal URLs
 *
 * Caller provides: 
 *  page pointer to webpage_t struct
 *  pagesToCrawl pointer to bag_t struct that keeps track of pages to be crawled
 *  pagesSeen pointer to hashtable_t struct that keeps track of URLs already visited
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  // Initialize position (in HTML) and URL variables
  int pos = 0;
  char* URL  = NULL;

  // Enqueue the processing of relevant URLs
  while ((URL = webpage_getNextURL(page, &pos)) != NULL) {
    char* normalURL = normalizeURL(URL);
    printf("%d\tFound: %s\n", webpage_getDepth(page), normalURL);

    // Ensure the URL is internal
    if (isInternalURL(normalURL) == false) {
      printf("%d\tIgnExtrn: %s\n", webpage_getDepth(page), normalURL);
      free(URL);
      free(normalURL);
      continue;
    }

    // Ensure the URL has not been visited already
    if (hashtable_insert(pagesSeen, normalURL, "") == false) { 
      printf("%d\tIgnDupl: %s\n", webpage_getDepth(page), normalURL);
      free(URL);
      free(normalURL);
      continue;
    }

    // Initialize and mark it as a webpage to crawl
    bag_insert(pagesToCrawl, webpage_new(normalURL, webpage_getDepth(page) + 1, NULL));
    printf("%d\tAdded: %s\n", webpage_getDepth(page), normalURL);
    
    free(URL); // only free URL since webpage_delete will eventually take care of normalURL
  }
}
