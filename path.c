#include "shell.h"

/**
 * isCmd - Determines if a file is an executable command
 * @info: The info struct
 * @path: Path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int isCmd(shell_info *info, char *path)
{
    struct stat st;

    (void)info;
    if (!path || stat(path, &st))
        return (0);

    if (st.st_mode & S_IFREG)
        return (1);

    return (0);
}

/**
 * dupChars - Duplicates characters
 * @pathstr: The PATH string
 * @start: Starting index
 * @stop: Stopping index
 *
 * Return: Pointer to a new buffer
 */
char *dupChars(const char *pathstr, int start, int stop)
{
    static char buf[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)
    {
        if (pathstr[i] != ':')
            buf[k++] = pathstr[i];
    }
    buf[k] = '\0';
    return (buf);
}

/**
 * findPath - Finds this cmd in the PATH string
 * @info: The info struct
 * @pathstr: The PATH string
 * @cmd: The cmd to find
 *
 * Return: Full path of cmd if found or NULL
 */
char *findPath(shell_info *info, char *pathstr, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!pathstr)
        return (NULL);
    if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
    {
        if (isCmd(info, cmd))
            return (cmd);
    }
    while (1)
    {
        if (!pathstr[i] || pathstr[i] == ':')
        {
            path = dupChars(pathstr, curr_pos, i);
            if (!*path)
                _strcat(path, cmd);
            else
            {
                _strcat(path, "/");
                _strcat(path, cmd);
            }
            if (isCmd(info, path))
                return (path);
            if (!pathstr[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return (NULL);
}