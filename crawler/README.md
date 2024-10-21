I assumed that, on error, we do not really care about freeing whatever memory has been allocated.
    e.g. I do not free `seedURL` on errors in `parseArgs`
    e.g. I do not free `seedURL` on error while creating a hashtable in `crawl`

My implementation (differs?) from the spec in that, in `pageScan`, I do not check if the URL is internal, but rather if it is external. I do a similar thing for the operation of inserting the URL into the hashtable. I find that this is more convenient for logging the correct status (IgnExtrn and IgnDupl). Makes code more readable and easy to follow logically.

My implementation fails to work (at least expectedly) on `maxDepth` arguments that have more than 5 digits, by choice.
