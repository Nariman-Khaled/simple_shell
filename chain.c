#include "shell.h"
#include <stdlib.h>

/**
 * checkCommandChain - Test if the current character in buffer is a chain delimiter
 * @shellInfo: The parameter struct
 * @buffer: The character buffer
 * @position: Address of the current position in buffer
 *
 * Return: 1 if a chain delimiter, 0 otherwise
 */
int checkCommandChain(shell_info *shellInfo, char *buffer, size_t *position)
{
    size_t index = *position;

    switch (buffer[index]) {
        case '|':
            if (buffer[index + 1] == '|') {
                buffer[index] = '\0';
                index++;
                shellInfo->cmd_buf_type = CMD_OR;
                break;
            }
            return (0);

        case '&':
            if (buffer[index + 1] == '&') {
                buffer[index] = '\0';
                index++;
                shellInfo->cmd_buf_type = CMD_AND;
                break;
            }
            return (0);

        case ';':
            buffer[index] = '\0';
            shellInfo->cmd_buf_type = CMD_CHAIN;
            break;

        default:
            return (0);
    }

    *position = index;
    return (1);
}

/**
 * check_chain - checks if command chaining should continue based on last status
 * @info: shell information containing command buffer type and execution status
 * @buf: character buffer to be modified based on command type and status
 * @position: pointer to the current position in the buffer
 * @startIndex: starting position in the buffer for the current command
 * @bufferLength: total length of the buffer
 *
 * This function modifies the buffer and updates the position based on the type
 * of command chaining (CMD_AND, CMD_OR) and the execution status of the last
 * command. It sets the buffer at the starting index to null and updates the
 * position to the end of the buffer if the conditions for chaining are not met.
 */
void check_chain(shell_info *info, char *buf, size_t *position, size_t startIndex, size_t bufferLength)
{
    size_t currentPosition = *position;

    if ((info->cmd_buf_type == CMD_AND && info->status) ||
        (info->cmd_buf_type == CMD_OR && !info->status)) {
        buf[startIndex] = '\0';
        currentPosition = bufferLength;
    }

    *position = currentPosition;
}

/**
 * replace_alias - Replaces an alias in the tokenized string
 * @info: The parameter struct containing shell information
 *
 * Description: Iterates through the aliases list. If a matching alias is found,
 * the first argument in the argv array is replaced with the alias's value.
 * The function stops after a maximum of 10 iterations or once an alias is replaced.
 *
 * Return: 1 if an alias is replaced, 0 otherwise
 */
int replace_alias(shell_info *info)
{
    int iteration;
    list_t *node;
    char *alias_value;

    for (iteration = 0; iteration < 10; iteration++) {
        node = nodeStartsWith(info->alias, info->argv[0], '=');
        if (!node)
            return (0);

        alias_value = _strchr(node->str, '=');
        if (!alias_value)
            return (0);

        alias_value = _strdup(alias_value + 1);
        if (!alias_value)
            return (0);

        free(info->argv[0]);
        info->argv[0] = alias_value;
        break;
    }
    return (1);
}

/**
 * replace_shell_variables - Replaces variables in the tokenized string
 * @info: The parameter struct containing shell information
 *
 * Description: Iterates through the arguments array (argv). If a variable
 * is identified (starting with $), the function attempts to replace it
 * with its value from the environment or special variables.
 *
 * Return: 1 if at least one variable is replaced, 0 otherwise
 */
int replace_shell_variables(shell_info *info)
{
    int i, replaced = 0;
    list_t *node;

    for (i = 0; info->argv[i]; i++) {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!_strcmp(info->argv[i], "$?")) {
            replace_string(&(info->argv[i]), _strdup(convert_number(info->status, 10, 0)));
            replaced = 1;
            continue;
        }

        if (!_strcmp(info->argv[i], "$$")) {
            replace_string(&(info->argv[i]), _strdup(convert_number(getpid(), 10, 0)));
            replaced = 1;
            continue;
        }

        node = nodeStartsWith(info->env, &info->argv[i][1], '=');
        if (node) {
            replace_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
            replaced = 1;
            continue;
        }

        replace_string(&info->argv[i], _strdup(""));
        replaced = 1;
    }

    return (replaced);
}

/**
 * replace_string - Replaces a string with a new one
 * @old: Address of the old string
 * @new: The new string to replace with
 *
 * Description: Frees the memory allocated for the old string and assigns
 * the new string to its place. If the old string is NULL, no action is taken.
 *
 * Return: 1 if replaced successfully, 0 if the old string is NULL
 */
int replace_string(char **old, char *new)
{
    if (old == NULL)
        return 0;

    free(*old);
    *old = new;
    return 1;
}
