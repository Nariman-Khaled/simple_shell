#include "shell.h"

/**
 * interactive - Determines if the shell is in interactive mode.
 * @info: Pointer to the shell_info structure.
 *
 * Return: 1 if in interactive mode, 0 otherwise.
 */
int interactive(shell_info *info)
{
    return isatty(STDIN_FILENO) && (info->readfd == STDIN_FILENO);
}

/**
 * is_delim - Checks if a character is a delimiter.
 * @c: The character to check.
 * @delim: The delimiter string.
 *
 * Return: 1 if c is a delimiter, 0 otherwise.
 */
int is_delim(char c, char *delim)
{
    if (!delim)
        return (0);

    while (*delim)
    {
        if (*delim == c)
            return (1);
        delim++;
    }

    return (0);
}

/**
 * _isalpha - Checks for an alphabetic character.
 * @c: The character to check.
 *
 * Return: 1 if c is alphabetic, 0 otherwise.
 */
int _isalpha(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/**
 * prune_history - Prunes the shell's history to a maximum number of entries.
 * @info: The parameter struct containing the history list.
 * @max_entries: The maximum number of history entries to retain.
 */
void prune_history(shell_info *info, int max_entries)
{
    list_t *current;
    int count = 0;

    /* Count the number of entries in the history */
    for (current = info->history; current; current = current->next)
        count++;

    /* Remove the oldest entries if the count exceeds max_entries */
    while (count-- > max_entries)
        remove_node_at(&(info->history), 0);
}
