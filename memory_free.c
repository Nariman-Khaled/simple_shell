#include "shell.h"
#include <stdlib.h>


/**
 * ffree - Frees a dynamically allocated array of strings
 * @strings: Pointer to the first string in the array
 *
 * Description: Iterates through each string in the array, freeing each one,
 * then frees the array itself. Assumes the array is NULL-terminated.
 */

void ffree(char **strings)
{
    char **original_ptr = strings;

    if (!strings)
        return;

    while (*strings)
        free(*strings++);

    free(original_ptr);
}


/**
 * bfree - Frees a pointer and NULLs the address
 * @ptr: Address of the pointer to free
 *
 * Description: Frees the memory pointed to by *ptr and sets *ptr to NULL.
 * Helps in preventing dangling pointer issues.
 *
 * Return: 1 if memory was freed, 0 if not (e.g., ptr or *ptr is NULL).
 */

int bfree(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
        return 1;
    }
    return 0;
}

