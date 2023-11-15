#include <signal.h>
#include "shell.h"
#include <stdlib.h>

/**
 * load_input_to_buffer - Buffers chained commands.
 * @info: Parameter struct containing shell information.
 * @buf: Address of the buffer.
 * @len: Address of the length variable.
 *
 * Return: Number of bytes read.
 */
ssize_t load_input_to_buffer(shell_info *info, char **buf, size_t *len)
{
    ssize_t bytes_read = 0;
    size_t buffer_length = 0;

    /* Return immediately if the buffer still has data */
    if (*len > 0)
        return bytes_read;

    /* Free and reset the buffer */
    free(*buf);
    *buf = NULL;

    /* Set signal handler for SIGINT */
    signal(SIGINT, handle_ctrl_c_signal);

    /* Read input based on configuration (getline or custom custom_getline) */
#if USE_GETLINE
    bytes_read = getline(buf, &buffer_length, stdin);
#else
    bytes_read = custom_getline(info, buf, &buffer_length);
#endif

    /* Process input if any data is read */
    if (bytes_read > 0)
    {
        /* Check and remove trailing newline */
        if ((*buf)[bytes_read - 1] == '\n')
        {
            (*buf)[bytes_read - 1] = '\0';
            bytes_read--;
        }

        /* Set flags and process the buffer */
        info->linecount_flag = 1;
        remove_comments(*buf);
        append_to_history(info, *buf, info->histcount++);

        /* Check for command chain and update info */
        if (_strchr(*buf, ';'))
        {
            *len = bytes_read;
            info->cmd_buf = buf;
        }
    }

    return bytes_read;
}



/**
 * retrieve_input_line - Gets a line minus the newline, handling command chains.
 * @info: Parameter struct containing shell information.
 *
 * Return: Number of bytes read, or -1 on EOF.
 */
ssize_t retrieve_input_line(shell_info *info)
{
    static char *buf; /* Command chain buffer */
    static size_t buf_pos = 0, chain_pos = 0, buf_len = 0;
    ssize_t bytes_read = 0;
    char **current_cmd = &(info->arg), *cmd_start;

    _putchar(BUF_FLUSH);
    bytes_read = load_input_to_buffer(info, &buf, &buf_len);

    if (bytes_read == -1) /* EOF handling */
        return (-1);

    if (buf_len) /* Commands left in the chain buffer */
    {
        chain_pos = buf_pos; /* Initialize new iterator to current buffer position */
        cmd_start = buf + buf_pos; /* Get pointer to the current command */

        check_chain(info, buf, &chain_pos, buf_pos, buf_len);

        /* Iterate to semicolon or end of buffer */
        while (chain_pos < buf_len)
        {
            if (checkCommandChain(info, buf, &chain_pos))
                break;
            chain_pos++;
        }

        buf_pos = chain_pos + 1; /* Increment past the nullified semicolon */

        if (buf_pos >= buf_len) /* Check if end of buffer is reached */
        {
            buf_pos = buf_len = 0; /* Reset buffer position and length */
            info->cmd_buf_type = CMD_NORM;
        }

        *current_cmd = cmd_start; /* Pass back pointer to the current command */
        return (_strlen(cmd_start)); /* Return the length of the current command */
    }

    *current_cmd = buf; /* Pass back the buffer from load_input_to_buffer */
    return (bytes_read); /* Return the length from load_input_to_buffer */
}

/**
 * read_data_to_buffer - Reads data into a buffer from a file descriptor.
 * @info: Parameter struct containing the file descriptor to read from.
 * @buf: Buffer where the data will be stored.
 * @bytes_read: Pointer to the size variable, updated with the number of bytes read.
 *
 * Return: The number of bytes read, or -1 on error.
 */
ssize_t read_data_to_buffer(shell_info *info, char *buf, size_t *bytes_read)
{
    ssize_t read_result = 0;

    /* Check if the buffer already has data */
    if (*bytes_read)
        return 0;

    /* Read data into the buffer */
    read_result = read(info->readfd, buf, READ_BUF_SIZE);

    /* Update the bytes_read if read operation was successful */
    if (read_result >= 0)
        *bytes_read = read_result;

    return read_result;
}


/**
 * custom_getline - Gets the next line of input from STDIN.
 * @info: Parameter struct containing read file descriptor.
 * @ptr: Address of the pointer to the buffer, preallocated or NULL.
 * @length: Size of preallocated buffer if not NULL, updated with new size.
 *
 * Return: The number of characters read, or -1 on error or EOF.
 */
int custom_getline(shell_info *info, char **ptr, size_t *length)
{
    static char buf[READ_BUF_SIZE];
    static size_t current_pos = 0, buf_len = 0;
    size_t line_len;
    ssize_t read_result = 0, total_chars = 0;
    char *line_start = NULL, *new_buffer = NULL, *newline_char;

    line_start = *ptr;
    if (line_start && length)
        total_chars = *length;

    if (current_pos == buf_len)
        current_pos = buf_len = 0;

    read_result = read_data_to_buffer(info, buf, &buf_len);
    if (read_result == -1 || (read_result == 0 && buf_len == 0))
        return (-1);

    newline_char = _strchr(buf + current_pos, '\n');
    line_len = newline_char ? (size_t)(1 + newline_char - buf) : buf_len;
    new_buffer = reAllocate(line_start, total_chars, total_chars ? total_chars + line_len : line_len + 1);
    if (!new_buffer)
        return (line_start ? free(line_start), -1 : -1);

    if (total_chars)
        _strncat(new_buffer, buf + current_pos, line_len - current_pos);
    else
        _strncpy(new_buffer, buf + current_pos, line_len - current_pos + 1);

    total_chars += line_len - current_pos;
    current_pos = line_len;
    *ptr = new_buffer;

    if (length)
        *length = total_chars;
    return (total_chars);
}

/**
 * handle_ctrl_c_signal - Handles the SIGINT signal (Ctrl-C).
 * @sig_num: The signal number (unused in this function).
 *
 * Return: void
 */
void handle_ctrl_c_signal(__attribute__((unused)) int sig_num)
{
    _puts("\n$ ");  /* Print a new line and prompt */
    _putchar(BUF_FLUSH);  /* Flush the output buffer */
}

