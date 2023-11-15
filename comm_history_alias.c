#include "shell.h"
#include <stdlib.h>

/**
 * shell_history - Displays the history list with line numbers.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *
 * Description: Utilizes the printList function to display the history
 * of commands executed in the shell, each preceded by a line number,
 * starting from 0.
 *
 * Return: Always returns 0.
 */
int shell_history(shell_info *info)
{
    if (info && info->history)
    {
        printList(info->history);
    }
    else
    {
        _puts("No history available.\n");
    }

    return (0);
}


/**
 * unsetAlias - Removes an alias from the alias list.
 * @info: The parameter struct containing shell information.
 * @str: The alias name to be removed.
 *
 * Description: Deletes an alias from the alias list based on the provided alias name.
 * An alias is identified by its name before the '=' character.
 *
 * Return: 0 on successful removal, 1 if the alias does not exist.
 */
int unsetAlias(shell_info *info, char *str)
{
    int node_index;

    /* Get the index of the node with the matching alias name */
    node_index = get_node_index(info->alias, nodeStartsWith(info->alias, str, '='));
    if (node_index == -1)
        return (1); /* Alias not found */

    /* Remove the alias node */
    return remove_node_at(&(info->alias), node_index) == 0 ? 0 : 1;
}


/**
 * setAlias - Sets or updates an alias.
 * @info: Parameter struct containing shell information.
 * @str: The string representing the alias in the format 'name=value'.
 *
 * Description: Sets or updates an alias. If the value part is empty,
 * the alias is removed. The alias is added to the end of the alias list.
 *
 * Return: 0 on success, 1 on error.
 */
int setAlias(shell_info *info, char *str)
{
    char *alias_value;

    /* Find the position of the '=' character */
    alias_value = _strchr(str, '=');
    if (!alias_value || !*(alias_value + 1))
        return (unsetAlias(info, str)); /* Unset the alias if value part is empty */

    /* Unset the existing alias before setting the new one */
    unsetAlias(info, str);

    /* Add the new alias to the end of the alias list */
    return (append_node(&(info->alias), str, 0) == NULL);
}


/**
 * printAlias - Prints an alias string.
 * @node: The alias node to be printed.
 *
 * Description: Prints the alias in the format 'name=value', where 'name'
 * is the alias name and 'value' is the associated command string.
 *
 * Return: 0 on success, 1 if the node is NULL or no '=' found.
 */
int printAlias(list_t *node)
{
    char *alias_separator;
    char *a;

    if (!node)
        return (1);

    alias_separator = _strchr(node->str, '=');
    if (!alias_separator)
        return (1);

    for (a = node->str; a <= alias_separator; a++)
        _putchar(*a);

    _putchar('\'');
    _puts(alias_separator + 1);
    _puts("'\n");

    return (0);
}



/**
 * alias - Mimics the alias builtin command.
 * @info: Structure containing potential arguments.
 *
 * Description: If no arguments are provided, it prints all aliases.
 * If an argument is provided, it either sets a new alias or prints
 * a specific existing alias.
 *
 * Return: Always returns 0.
 */
int alias(shell_info *info)
{
    int i;
    char *alias_assignment;
    list_t *current_node;

    /* Print all aliases if no additional arguments are provided */
    if (info->argc == 1)
    {
        for (current_node = info->alias; current_node; current_node = current_node->next)
        {
            printAlias(current_node);
        }
    }
    else
    {
        /* Process each argument */
        for (i = 1; info->argv[i]; i++)
        {
            alias_assignment = _strchr(info->argv[i], '=');

            if (alias_assignment)
                setAlias(info, info->argv[i]);  /* Set new alias */
            else
                printAlias(nodeStartsWith(info->alias, info->argv[i], '='));  /* Print specific alias */
        }
    }

    return (0);
}

