I assume that all files in the `pageDirectory` provided to `indexer` (should it pass tests in parseArgs) are crawler-produced, so that while incrementing `docID` to read each and every webpage file, as soon as we cannot read one file, it means that we have already processed all webpage files in `pageDirectory`, so we stop reading webpage files.

I assume that there is no file in `pageDirectory` whose filename is a number of more than 5 digits.
