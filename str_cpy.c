#include "shell.h"

/**
 * _strcpy - copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *_strcpy(char *dest, const char *src)
{
	int i = 0;

	if (dest == src || src == 0)
		return (dest);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

/**
 * _strncpy - Copies up to n bytes of the string pointed to by src
 *            to the buffer pointed to by dest.
 * @dest: The buffer to copy to.
 * @src: The string to copy from.
 * @n: The maximum number of bytes to copy.
 *
 * Return: A pointer to the destination string dest.
 */
char *_strncpy(char *dest, const char *src, int n)
{
    int i;

    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }

    /* If src has less than n bytes, fill the rest of dest with null bytes */
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }

    return (dest);
}