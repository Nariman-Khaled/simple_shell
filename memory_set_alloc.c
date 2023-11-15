#include "shell.h"
#include <stdlib.h>

/**
 * _memset - Fills memory with a constant byte
 * @s: Pointer to the memory area
 * @b: Byte to fill *s with
 * @n: Number of bytes to be filled
 *
 * Return: Pointer to the memory area s
 */

char *_memset(char *s, char b, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++)
        s[i] = b;

    return s;
}

/**
 * reAllocate - Reallocates a block of memory
 * @ptr: Pointer to previous malloc'ed block
 * @old_size: Byte size of previous block
 * @new_size: Byte size of new block
 *
 * Return: Pointer to the newly allocated block, or NULL on failure
 */

void *reAllocate(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *new_ptr;
    unsigned int i, min_size;

    if (!ptr)
        return malloc(new_size);
    if (!new_size)
    {
        free(ptr);
        return NULL;
    }
    if (new_size == old_size)
        return ptr;

    new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;

    min_size = (old_size < new_size) ? old_size : new_size;
    for (i = 0; i < min_size; i++)
        new_ptr[i] = ((char *)ptr)[i];

    free(ptr);
    return new_ptr;
}
