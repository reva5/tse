For `pagedir_init`, I assumed that caller wants program to crash (cleanly) if:
    `pageDirectory` is NULL

For `pagedir_save`, I assumed that caller wants program to crash (cleanly) if:
    any pointer argument is NULL
    file to write page information to cannot be opened
