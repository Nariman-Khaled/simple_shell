#include "shell.h"

/**
 * _strlen - Returns the length of a string.
 * @s: The string to get the length of.
 *
 * Return: The length of the string.
 */
int _strlen(const char *s)
{
    int length = 0;

    while (*s != '\0')
    {
        length++;
        s++;
    }

    return (length);
}

/**
 * _strcmp - Compares two strings.
 * @s1: First string for comparison.
 * @s2: Second string for comparison.
 *
 * Return: Difference between the ASCII values of the first pair of characters
 *         that are different in s1 and s2.
 */
int _strcmp(char *s1, char *s2)
{
    int i = 0;

    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
        {
            return (s1[i] - s2[i]);
        }
        i++;
    }

    /* Case when strings have different lengths */
    if (s1[i] != '\0')
    {
        return (s1[i]);
    }
    if (s2[i] != '\0')
    {
        return (-s2[i]);
    }

    /* Strings are equal */
    return (0);
}

/**
 * starts_with - checks if needle starts with haystack
 * @haystack: string to search
 * @needle: the substring to find
 *
 * Return: address of next char of haystack or NULL
 */
char *starts_with(const char *haystack, const char *needle)
{
	while (*needle)
		if (*needle++ != *haystack++)
			return (NULL);
	return ((char *)haystack);
}

/**
 * _strcat - concatenates two strings
 * @dest: the destination buffer
 * @src: the source buffer
 *
 * Return: pointer to destination buffer
 */
char *_strcat(char *dest, char *src)
{
	char *ret = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (ret);
}
