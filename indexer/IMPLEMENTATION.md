### CS50 TSE Indexer

## Implementation Spec
In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. The knowledge unit noted that an implementation spec may include many topics; not all are relevant to the TSE or the Crawler. Here we focus on the core subset:

- Data structures
- Control flow: pseudo code for overall flow, and for each of the functions
- Detailed function prototypes and their parameters
- Error handling and recovery
- Testing plan

## Data structures
We use two data structures: a 'counters' structure, which keeps count of the number of occurrences of a word for each document ID, and a 'hashtable', which maps from words to their respective 'counters'. These two data structures shall be wrapped in a single data structure called an 'index'.

When building an index from a page directory, the size of the hashtable (slots) is impossible to determine in advance since we do not know the amount of data we will have to process, so we use 600. When loading an index from an index file, it is possible to know in advance the number of words we will load (by reading the number of lines), so we assign a number of (number of words // 10) slots to the hashtable.

## Control flow
The Indexer is implemented in one file `indexer.c`, with four functions.

# main
The `main` function validates correct usage of program (e.g. correct number of command-line arguments), calls `parseArgs`, builds an index from a given webpage directory, saves that index to a given file, then deletes that index from memory. Finally it exits 0.

# parseArgs
Given arguments from the command line, extract them into this function's parameters; return only if successful.
- for `pageDirectory`, validate that it is crawler-produced (it has a `.crawler` writable file and at least a `1` file)
- for `indexFilename`, check if it can be created/opened in write mode

# indexBuild
Build an in-memory index from a given webpage directory. Pseudocode:
```
creates a new 'index' object
loops over document ID numbers, counting from 1
loads a webpage from the document file 'pageDirectory/id'
if successful, 
  passes the webpage and docID to indexPage
```

# indexPage
Scan a webpage file to add its words to the index. Pseudocode:
```
step through each word of the webpage,
    skips trivial words (less than length 3),
    normalizes the word (converts to lower case),
    looks up the word in the index,
        adding the word to the index if needed
    increments the count of occurrences of this word in this docID
```

The index tester is implemented in one file `indextest.c`, with one function.

# main
The `main` function validates correct usage of program (e.g. correct number of command-line arguments), parses arguments by checking that `oldIndexFilename` is readable and that `newIndexFilename` is writable, loads index in memory from `oldIndexFilename`, saves it to `newIndexFilename`, then deletes the index in memory. Finally it returns 0.

## Other modules
# pagedir
We extend the `pagedir` module to add functionality for validating that a webpage directory is crawler-produced and for loading a crawler-produced webpage file into a webpage struct. We thus add two new functions `pagedir_validate` and `pagedir_load`.

Pseudocode for `pagedir_validate`:
```
construct pathname for '.crawler' file in pageDirectory
construct pathname for '1' file in pageDirectory
check if files are accessible in read mode
```

Pseudocode for `pagedir_load`:
```
convert docID to string
construct pathname for webpage file in pageDirectory corresponding to docID
open webpage file
read each line into its corresponding variable (URL, depth, HTML, in that order)
convert depth to integer
close webpage file
return webpage struct built from URL, depth, HTML variables
```

# index
To represent an index in memory, we write a module defining an 'index\_t' data structure, along with various functions capturing all necessary functionality. This index data structure shall have only one field for a 'words' hashtable, that maps from words to 'counters' structures, which in turn map from document IDs to the number of occurrences of that word in that document. The functions to include are as follows: `index_new`, `index_add`, `index_set`, `index_load`, `index_save`, `index_delete`.

Pseudocode for `index_new`:
```
allocate memory for index_t struct
initialize words hashtable with given number of slots
return pointer to index_t struct
```

Pseudocode for `index_add`:
```
get counter set of word
    if no counter set, create one and insert it into hashtable
add one to word counter corresponding to docID
```

Pseudocode for `index_set`:
```
get counter set of word
    if no counter set, create one and insert it into hashtable
set word counter corresponding to docID to count
```

Pseudocode for `index_load`:
```
open index file; on error, return NULL
get number of lines (words) in index file
initialize index_t struct with number of slots being a tenth of number of words
step through each line of index file,
    read word at the beginning of each file into variable
    until there are no more (docID, count) pairs to pull off,
        read (docID, count) pair into two variables
        then set word counter corresponding to docID to count
    free word
close index file
return index
```

Pseudocode for `index_save`:
```
open index file; on error, do nothing
iterate over each (word, counters) pair,
    print 'word ' to file
    iterate over each (key, counter) pair,
        print 'key count ' to file
    print newline to file
```

Pseudocode for `index_delete`:
```
delete words hashtable
free memory for index
```

# word
We write this module to provide functionality for normalizing a word (necessary for `indexPage`). We define the function `word_normalizeWord`.

Pseudocode for `word_normalizeWord`:
```
loop through all word characters until we hit NULL char,
    convert character to lowercase (in place)
```

# libcs50
We leverage the modules of libcs50, most notably `counters` and `hashtable` in the definition of our index\_t structure (as already mentioned). We also make use of the `webpage` module in e.g. building a webpage\_t structure out of a webpage file in `pagedir_load`. Module `file` is used for getting the number of lines and reading lines in `index_load`. Finally, module `mem` is used for various calls to `mem_asset` that make sure memory was correctly allocated.

## Function prototypes
# indexer
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in indexer.c and is not repeated here.
```c
int main(const int argc, char* argv[]);
static void parseArgs(char* pageDirectory, char* indexFilename);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, int docID);
```

# pagedir 
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in pagedir.h and is not repeated here.
```c
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

# index
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in index.h and is not repeated here.
```c
index_t* index_new(int numSlots);
void index_add(index_t* index, char* word, int docID);
void index_set(index_t* index, char* word, int docID, int count);
void index_save(index_t* index, char* indexFilename);
index_t* index_load(char* indexFilename);
void index_delete(index_t* index);
```

# word
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in word.h and is not repeated here.
```c
void word_normalizeWord(char* word);
```

# indextest
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in indextest.c and is not repeated here.
```c
int main(const int argc, char* argv[]);
```

## Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem\_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem\_assert functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, pagedir\_validate returns false if there is no readable '.crawler' and '1' files, allowing the Indexer to decide what to do; `index_load` returns false if `indexFilename` is not readable; etc.


## Testing plan
Here is an implementation-specific testing plan.

# Unit testing
There are four units (indexer, pagedir, index, word). For the index module, a program 'indextest' will serve as a unit test. This program 'indextest', compiled from 'indextest.c', reads an index file into an index data structure using `index_load`, then writes the index out to a new index file using `index_save`. The other units are relatively tiny; they could be tested using a small C 'driver' to invoke their functions with various arguments, but it is likely sufficient to observe their behavior during the system test.

# Integration/system testing
We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.

First, a sequence of invocations with erroneous arguments, including 1. no arguments 2. one argument 3. three or more arguments 4. unexistent pageDirectory 5. existent, non-crawler-produced pageDirectory 6. existent, crawler-initialized but not produced pageDirectory 7. unexistent pathname indexFilename 8. read-only directory indexFilename 9. unwritable file indexFilename.

Second, a run with valgrind over a moderate-sized test case (such as toscrape at depth 1) for both `indexer` and `indextest`.

Third, run `indexer` on all pageDirectories created by crawler runs on all three CS50 websites (`letters` at depths 0, 1, 2, 3, `toscrape` at depths 0, 1, 2, 3, `wikipedia` at depths 0, 1). Run 'indextest' on those index files. Finally, run `indexcmp` on both the index file created and the corresponding sample output in the 'shared' directory.

Run that script with bash -v testing.sh so the output of crawler is intermixed with the commands used to invoke the crawler. Verify correct behavior by studying the output, and by sampling the files created
