# CS50 TSE Querier

## Implementation Spec

### Data structures
We use three main data structures: `tokens`, which represents the tokenization of a given query; `index`, which is loaded from `indexFilename`; and `counters`, which, for a given word in `index`, holds a map from docID to #occurrences. 

### Control flow
The Querier is implemented in one file `querier.c`, with six main functions but thirteen functions overall.

#### main
Pseudocode:
```
validate correct number of arguments
    print usage message otherwise and exit
call parseArgs
load index
while query != EOF,
    call respondQuery
delete index
```

#### parseArgs
Pseudocode:
```
call pagedir_validate on pageDirectory
    if false, print error message and exit
try to open indexFilename on read mode
    if file pointer NULL, print error message and exit
close indexFilename
```

#### parseQuery
Pseudocode:
```
iterate over each character in query,
    check if character is either alphabetic or whitespace
        if not, return false
return true
```

#### parseTokens
Pseudocode:
```
check if first token is an operator ("or" or "and")
    if it is, print error message and exit
check if last token is an operator
    if it is, print error message and exit
iterate over each token,
    check if current token is operator
        if so, 
            if last one was too, then print error message and exit
            if last one was not, proceed with next iter but remember this one was operator
        if not,
            proceed with next iter
```

#### processQuery
Pseudocode:
```
create page-score counters 'pages'
iterate over query tokens,
    create 'temp' counters that will temporarily hold the intersection of the 'andsequence'
    call unionWords on 'temp' and current token counters
    go to next token
    iterate over 'andsequence',
        if token is 'and',
            ignore and proceed with next
        call intersectWords on temp and current token counters (let temp hold result)
    call unionWords on 'pages' and 'temp' (let pages hold result)
    delete temp
return 'pages'
```

#### rankPages
Pseudocode:
```
(let 'pages' be the counters that map docID to a score)
create an array 'maxPageCount' that holds the key that corresponds to the highest count and its corresponding count
call counters_iterate on 'pages' with 'maxPageCount' as arg and 'countermax' as itemfunc
in 'pages', set key's count to 0
while key's (pulled) count is not 0,
    open corresponding page file and read the URL 
    print docID, score, and URL of page in the format "score\t[score] doc\t[docID]: [URL]"
    call counters_iterate on 'pages' with 'maxPageCount' as arg and 'countermax' as itemfunc
    in 'pages', set key's score to 0
```

#### respondQuery
Pseudocode:
```
call prompt
read query
if query is EOF,
    return 2 (signaling to terminate program)
call parseQuery
    if false, return 1 (signaling to prompt for next query)
tokenize query
check tokens (query) is not empty
    if it is, return 1
call parseTokens,
    if false, return 1
print clean query from tokens
call processQuery
count number of pages,
    if 0, print "No documents match"
    else, print "Matches [pageCount] documents (ranked):"
call rankPages
free malloc'd query, delete tokens, delete counters returned by queryPages
return 0
```

The following functions are 'helpers' to the previous functions:

#### intersectWords
Pseudocode:
```
create counters variable 'result' to hold intersection
call counters_iterate on 'wordBCounters' with bundle('wordACounters', 'result') as arg and 'countermin' as itemfunc
free pointer to 'wordACounters'
set pointer to 'wordACounters' to 'result
```

#### unionWords
Pseudocode:
```
call counters_iterate on 'wordBCounters' with 'wordACounters' as arg and 'countersum' as itemfunc
```

#### prompt
Pseudocode:
```
if stdin is a tty (terminal),
    print a prompt for query
```

#### countermin
Pseudocode:
```
cast void* arg accordingly
check that key is in the other counters,
    if it is, in 'result' counters, set key to be the lower count between the two
```

#### countersum
Pseudocode:
```
cast void* arg accordingly
in the other counters, set key to hold the sum of its count in this counters and the other counters
```

#### countermax
Pseudocode:
```
```

#### countercount
Pseudocode:
```
```

### Other modules
#### tokens
#### pagedir
#### index
#### libcs50

### Function prototypes
#### querier
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in querier.c and is not repeated here.
```c
int main(const int argc, char* argv[]);
static void parseArgs(char* pageDirectory, char* indexFilename);
static bool parseQuery(char* query);
static bool parseTokens(tokens_t* tokens);
static counters_t* processQuery(tokens_t* tokens, index_t* index);
static void rankPages(counters_t* pages, char* pageDirectory);
static int respondQuery(index_t* index, char* pageDirectory);

static void prompt(void);
static void intersectWords(counters_t** wordACounters, counters_t* wordBCounters);
static void unionWords(counters_t* wordACounters, counters_t* wordBCounters);

static void countermin(void* arg, const int key, const int count);
static void countersum(void* arg, const int key, const int count);
static void countermax(void* arg, const int key, const int count);
static void countercount(void* arg, const int key, const int count);
```
#### tokens
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in tokens.h and is not repeated here.
```c
int tokens_getLength(tokens_t* tokens);
tokens_t* tokens_new(int length);
char* tokens_get(tokens_t* tokens, int index);
void tokens_set(tokens_t* tokens, int index, char* token);
tokens_t* tokens_tokenize(char* query);
void tokens_delete(tokens_t* tokens);
```
#### pagedir
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in pagedir.h and is not repeated here.
```c
FILE* pagedir_open(const char* pageDirectory, const int docID, char* mode);
```
#### index
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in index.h and is not repeated here.
```c
counters_t* index_get(index_t* index, char* word);
```

### Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem\_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem\_assert functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `parseQuery` returns false if there are any bad characters in query, allowing the caller to determine what to do; similarly, there could be two adjacent operators in tokens when `parseTokens` is running, and the Querier does not treat that as a fatal error.

### Testing plan
Here is an implementation-specific testing plan.

#### Integration testing
We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments. It should test the following:

1. Test `querier` with various invalid arguments 1. no arguments, 2. one argument, 3. three or more arguments, 4. invalid pageDirectory (non-existent path) 5. invalid pageDirectory (not a crawler directory) 6. invalid indexFile (non-existent path) 7. invalid indexFile (read-only directory) 8. invalid indexFile (existing, write-only file)
2. Test `querier` with empty query and various invalid queries 1. operator in front 2. operator at the end 3. adjacent operators 4. non-alphabetic and non-whitespace character in query
3. Run `querier` on a variety of `pageDirectories` and their corresponding `indexFilename`. For each of these, run at least 10 queries generated by `fuzzquery` program, trying to make sure we get a decent-sized set of queries that more-or-less include: 1. a query that doesn't match any documents 2. a single word 3. multiple words (some explicit/implicit combination of: ) 4. a conjunction 5. a disjunction 6. a disjunction of conjunctions 7. disjunctions of conjuctions
4. Run `valgrind` on `querier` to ensure no memory leaks or errors
