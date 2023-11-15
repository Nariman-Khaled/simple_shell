#include "shell.h"
#include <stdlib.h>

/**
 * _strdup - Returns a pointer to a newly allocated space in memory,
 *          which contains a copy of the string given as a parameter.
 * @str: The string to duplicate.
 *
 * Return: Pointer to the duplicated string on success,
 *         NULL if str = NULL, or if insufficient memory was available.
 */
char *_strdup(const char *str)
{
    char *duplicate;
    unsigned int i, len = 0;

    if (str == NULL)
    {
        return (NULL);
    }

    while (str[len])
    {
        len++;
    }

    duplicate = malloc((len + 1) * sizeof(char));

    if (duplicate == NULL)
    {
        return (NULL);
    }

    for (i = 0; i < len; i++)
    {
        duplicate[i] = str[i];
    }

    duplicate[len] = '\0';

    return (duplicate);
}