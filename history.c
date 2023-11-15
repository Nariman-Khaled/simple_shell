#include "shell.h"

/**
 * getHistoryFile - Gets the history file path.
 * @info: Parameter struct containing environment variables.
 *
 * Return: Allocated string containing the history file path, or NULL on failure.
 */
char *getHistoryFile(shell_info *info)
{
    char *path_buffer, *home_dir;

    home_dir = getEnvironment(info, "HOME=");
    if (!home_dir)
        return (NULL);

    path_buffer = malloc(_strlen(home_dir) + _strlen(HISTORY_FILE) + 2);
    if (!path_buffer)
        return (NULL);

    _strcpy(path_buffer, home_dir);
    _strcat(path_buffer, "/");
    _strcat(path_buffer, HISTORY_FILE);

    return path_buffer;
}


/**
 * save_command_history - Writes the shell's command history to a file.
 * @info: The parameter struct containing history data.
 *
 * Return: 1 on successful write, -1 on failure.
 */
int save_command_history(shell_info *info)
{
    int fd;
    char *filename = getHistoryFile(info);
    list_t *current_node;

    if (!filename)
        return (-1);

    fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);

    if (fd == -1)
        return (-1);

    for (current_node = info->history; current_node; current_node = current_node->next)
    {
        _fd_puts(current_node->str, fd);
        _buffered_fd_putchar('\n', fd);
    }

    _buffered_fd_putchar(BUF_FLUSH, fd);
    close(fd);

    return 1;
}


/**
 * load_command_history - Reads the shell's command history from a file.
 * @info: The parameter struct containing history data.
 *
 * Return: The number of history entries read on success, 0 otherwise.
 */
int load_command_history(shell_info *info)
{
    int fd, linecount = 0;
    ssize_t rdlen;
    size_t fsize = 0;
    struct stat st;
    char *buf, *filename = getHistoryFile(info), *command_start;
    size_t i;

    if (!filename)
        return 0;

    fd = open(filename, O_RDONLY);
    free(filename);

    if (fd == -1 || fstat(fd, &st))
        return close(fd), 0;

    if (st.st_size < 0)
        return close(fd), 0;

    fsize = (size_t)st.st_size;
    if (fsize < 2)
        return close(fd), 0;

    buf = malloc(fsize + 1);
    if (!buf)
        return 0;

    rdlen = read(fd, buf, fsize);
    close(fd);

    if (rdlen <= 0)
        return free(buf), 0;

    buf[fsize] = '\0';
    for (i = 0, command_start = buf; i < fsize; i++)
    {
        if (buf[i] == '\n')
        {
            buf[i] = '\0';
            append_to_history(info, command_start, linecount++);
            command_start = buf + i + 1;
        }
    }

    if (command_start != buf + i)
        append_to_history(info, command_start, linecount++);

    free(buf);
    info->histcount = linecount;
    prune_history(info, HISTORY_MAX);
    renumberHistory(info);

    return info->histcount;
}



/**
 * append_to_history - Adds an entry to the history linked list.
 * @info: Structure containing potential arguments, including the history list.
 * @buf: The command line buffer to add to the history.
 * @linecount: The history line count (histcount).
 *
 * Return: Always 0.
 */
int append_to_history(shell_info *info, char *buf, int linecount)
{
    if (!info->history)
    {
        /* Initialize history list if it's empty */
        info->history = NULL;
    }

    /* Add the new history entry to the end of the list */
    append_node(&(info->history), buf, linecount);

    return 0;
}

/**
 * renumberHistory - Renumbers the history linked list after changes.
 * @info: Structure containing the history list.
 *
 * Return: The updated count of history entries.
 */
int renumberHistory(shell_info *info)
{
    list_t *current_node;
    int index = 0;

    for (current_node = info->history; current_node; current_node = current_node->next)
    {
        current_node->num = index++;
    }

    info->histcount = index;
    return index;
}

