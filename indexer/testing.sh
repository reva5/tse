## Invocations with erroneous arguments

# No arguments
./indexer

# One argument
./indexer arg1

# More than two arguments
./indexer arg1 arg2 arg3 arg4 arg5

# Unexistent pageDirectory
./indexer unexistent ../data/letters.index

# Existent, non-crawler-produced pageDirectory
./indexer test_dir ../data/letters.index

# Existent, crawler-initialized but not produced pageDirectory
./indexer test2_dir ../data/letters.index

# Unexistent pathname indexFilename
./indexer ../data/letters unexistent/unexistent

# Read-only directory indexFilename
./indexer ../data/letters test3_dir

# Unwritable file indexFilename
./indexer ../data/letters unwritable


## Run with valgrind over moderate-sized test case

valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/toscrape-1 ../data/toscrape-1.index
valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/toscrape-1.index ../data/toscrape-1-copy.index


## Runs over directories crawler-produced by all three CS50 websites, then compare with 'shared' index

# letters at depth 0
./indexer ~/cs50-dev/shared/tse/output/letters-0 ../data/letters-0.index
./indextest ../data/letters-0.index ../data/letters-0-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/letters-0.index ~/cs50-dev/shared/tse/output/letters-0.index

# letters at depth 1
./indexer ~/cs50-dev/shared/tse/output/letters-1 ../data/letters-1.index
./indextest ../data/letters-1.index ../data/letters-1-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/letters-1.index ~/cs50-dev/shared/tse/output/letters-1.index

# letters at depth 2
./indexer ~/cs50-dev/shared/tse/output/letters-2 ../data/letters-2.index
./indextest ../data/letters-2.index ../data/letters-2-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/letters-2.index ~/cs50-dev/shared/tse/output/letters-2.index

# letters at depth 3
./indexer ~/cs50-dev/shared/tse/output/letters-3 ../data/letters-3.index
./indextest ../data/letters-3.index ../data/letters-3-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/letters-3.index ~/cs50-dev/shared/tse/output/letters-3.index

# toscrape at depth 0
./indexer ~/cs50-dev/shared/tse/output/toscrape-0 ../data/toscrape-0.index
./indextest ../data/toscrape-0.index ../data/toscrape-0-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/toscrape-0.index ~/cs50-dev/shared/tse/output/toscrape-0.index

# toscrape at depth 1
./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../data/toscrape-1.index
./indextest ../data/toscrape-1.index ../data/toscrape-1-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/toscrape-1.index ~/cs50-dev/shared/tse/output/toscrape-1.index

# toscrape at depth 2
./indexer ~/cs50-dev/shared/tse/output/toscrape-2 ../data/toscrape-2.index
./indextest ../data/toscrape-2.index ../data/toscrape-2-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/toscrape-2.index ~/cs50-dev/shared/tse/output/toscrape-2.index

# toscrape at depth 3
./indexer ~/cs50-dev/shared/tse/output/toscrape-3 ../data/toscrape-3.index
./indextest ../data/toscrape-3.index ../data/toscrape-3-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/toscrape-3.index ~/cs50-dev/shared/tse/output/toscrape-3.index

# wikipedia at depth 0
./indexer ~/cs50-dev/shared/tse/output/wikipedia-0 ../data/wikipedia-0.index
./indextest ../data/wikipedia-0.index ../data/wikipedia-0-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/wikipedia-0.index ~/cs50-dev/shared/tse/output/wikipedia-0.index

# wikipedia at depth 1
./indexer ~/cs50-dev/shared/tse/output/wikipedia-1 ../data/wikipedia-1.index
./indextest ../data/wikipedia-1.index ../data/wikipedia-1-copy.index
~/cs50-dev/shared/tse/indexcmp ../data/wikipedia-1.index ~/cs50-dev/shared/tse/output/wikipedia-1.index

# wikipedia at depth 2 (commented out because some files in 'shared' output are not readable)
# ./indexer ~/cs50-dev/shared/tse/output/wikipedia-2 ../data/wikipedia-2.index
# ./indextest ../data/wikipedia-2.index ../data/wikipedia-2-copy.index
# ~/cs50-dev/shared/tse/indexcmp ../data/wikipedia-2.index ~/cs50-dev/shared/tse/output/wikipedia-2.index
