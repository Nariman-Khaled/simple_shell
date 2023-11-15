#include "shell.h"
#include <errno.h>

/**
 * _eputs - Prints an input string.
 * @str: The string to be printed.
 *
 * Return: Nothing.
 */
void _eputs(char *str)
{
    if (!str)
        return;

    while (*str)
    {
        _buffered_err_putchar(*str++);
    }
}

/**
 * _buffered_err_putchar - Writes the character c to stderr with buffering.
 * @c: The character to print.
 *
 * Return: On success 1. On error, -1 is returned, and errno is set appropriately.
 */
int _buffered_err_putchar(char c)
{
    static int i = 0;
    static char buf[WRITE_BUF_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        ssize_t written = write(2, buf, i);
        if (written == -1)
            return -1;
        i = 0;
    }

    if (c != BUF_FLUSH)
        buf[i++] = c;

    return 1;
}

/**
 * _buffered_fd_putchar - Writes the character c to the given file descriptor with buffering.
 * @c: The character to print.
 * @fd: The file descriptor to write to.
 *
 * Return: On success 1. On error, -1 is returned, and errno is set appropriately.
 */
int _buffered_fd_putchar(char c, int fd)
{
    static int i = 0;
    static char buf[WRITE_BUF_SIZE];
    ssize_t written;

    if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        written = write(fd, buf, i);
        if (written == -1)
            return -1;
        i = 0;
    }

    if (c != BUF_FLUSH)
        buf[i++] = c;

    return 1;
}

/**
 * _fd_puts - Prints an input string to a specified file descriptor.
 * @str: The string to be printed.
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters written.
 */
int _fd_puts(char *str, int fd)
{
    int i = 0;

    if (!str)
        return (0);

    while (*str)
    {
        i += _buffered_fd_putchar(*str++, fd);
    }

    return i;
}
