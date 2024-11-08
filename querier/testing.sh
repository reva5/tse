## Invocations with erroneous arguments

# No arguments
./querier

# One argument
./querier arg1

# More than two arguments
./querier arg1 arg2 arg3 arg4 arg5

# Unexistent pageDirectory
./querier unexistent ../data/letters.index

# Existent, non-crawler-produced pageDirectory
./querier test_dir ../data/letters.index

# Existent, crawler-initialized but not produced pageDirectory
./querier test2_dir ../data/letters.index

# Unexistent pathname indexFilename
./querier ../data/letters unexistent/unexistent

# Write-only directory indexFilename
./querier ../data/letters test3_dir

# Unreadable file indexFilename
./querier ../data/letters unreadable


## Invocations with empty query and invalid queries

# Operator in front
./querier ../data/letters ../data/letters.index < fuzzquery_files/fq-1

# Operator at the end
./querier ../data/letters ../data/letters.index < fuzzquery_files/fq-2

# Adjacent operators
./querier ../data/letters ../data/letters.index < fuzzquery_files/fq-3

# Non-alphabetic and non-whitespace character
./querier ../data/letters ../data/letters.index < fuzzquery_files/fq-4

# Empty query
./querier ../data/letters ../data/letters.index < fuzzquery_files/fq-5


## Run with valgrind over moderate-sized test case

valgrind --leak-check=full --show-leak-kinds=all ./querier ../data/toscrape-1 ../data/toscrape-1.index < fuzzquery_files/fq1


## Runs over pageDirectories and indexFilenames produced by all three CS50 websites

# letters at depth 0
./querier ~/cs50-dev/shared/tse/output/letters-0 ../data/letters-0.index < fuzzquery_files/fq2

# letters at depth 1
./querier ~/cs50-dev/shared/tse/output/letters-1 ../data/letters-1.index < fuzzquery_files/fq3

# letters at depth 2
./querier ~/cs50-dev/shared/tse/output/letters-2 ../data/letters-2.index < fuzzquery_files/fq4

# letters at depth 3
./querier ~/cs50-dev/shared/tse/output/letters-3 ../data/letters-3.index < fuzzquery_files/fq5

# toscrape at depth 0
./querier ~/cs50-dev/shared/tse/output/toscrape-0 ../data/toscrape-0.index < fuzzquery_files/fq6

# toscrape at depth 1
./querier ~/cs50-dev/shared/tse/output/toscrape-1 ../data/toscrape-1.index < fuzzquery_files/fq7

# toscrape at depth 2
./querier ~/cs50-dev/shared/tse/output/toscrape-2 ../data/toscrape-2.index < fuzzquery_files/fq8

# toscrape at depth 3
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ../data/toscrape-3.index < fuzzquery_files/fq9

# wikipedia at depth 0
./querier ~/cs50-dev/shared/tse/output/wikipedia-0 ../data/wikipedia-0.index < fuzzquery_files/fq10

# wikipedia at depth 1
./querier ~/cs50-dev/shared/tse/output/wikipedia-1 ../data/wikipedia-1.index < fuzzquery_files/fq11
