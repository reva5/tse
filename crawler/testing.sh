## Invocations with erroneous arguments

# No arguments
./crawler

# One argument
./crawler arg1

# Two arguments
./crawler arg1 arg2

# More than three arguments
./crawler arg1 arg2 arg3 arg4 arg5 arg6

# Non-numeric maxDepth 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters hello

# Unnormalizable seedURL
./crawler ,,,,thisisnotnormalizable291_-12\`z ../data/letters 2

# External seedURL
./crawler HTTP://UsEr:PaSs@www.EXAMPLE.com/path/.././file.html?name=val#top ../data/letters 2

# Unexistent pageDirectory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html doesntexist 2

# Out of range maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 11

## Run with valgrind over moderate-sized test case

valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-1 1

## Runs over all three CS50 websites

# letters at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 0

# letters at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-1 1

# letters at depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2

# letters at depth 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-10 10

# toscrape at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-0 0

# toscrape at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-1 1

# toscrape at depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-2 2

# toscrape at depth 3
#./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-3 3

# wikipedia at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-0 0

# wikipedia at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-1 1

# wikipedia at depth 2
#./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-2 2
