#include "shell.h"
#include <stdlib.h>

/**
 * shellEnvironment - Prints the current environment.
 * @info: Structure containing potential arguments, including the environment list.
 *
 * Description: Utilizes the printStringList function to print the environment
 * variables and their values, as maintained in the env list of the info structure.
 *
 * Return: Always returns 0.
 */
int shellEnvironment(shell_info *info)
{
    if (info && info->env)
    {
        printStringList(info->env);
    }
    else
    {
        _puts("No environment variables to display.\n");
    }

    return (0);
}


/**
 * getEnvironment - Gets the value of an environment variable.
 * @info: Structure containing potential arguments and the environment list.
 * @name: The name of the environment variable.
 *
 * Description: Searches the environment list for the specified variable
 * name and returns its value.
 *
 * Return: Pointer to the value of the environment variable, or NULL if not found.
 */
char *getEnvironment(shell_info *info, const char *name)
{
    list_t *node;
    char *value_start;
    size_t name_len;

    if (!info || !name)
        return (NULL);

    node = info->env;
    name_len = _strlen(name);

    while (node)
    {
        value_start = starts_with(node->str, name);
        if (value_start && value_start[name_len] == '=')
            return (value_start + name_len + 1);

        node = node->next;
    }

    return (NULL);
}


/**
 * _mysetenv - Initialize a new environment variable, or modify an existing one.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *
 * Description: Checks for the correct number of arguments and then calls
 * setEnvironment to set or modify the environment variable.
 *
 * Return: 0 on success, 1 on failure or incorrect argument count.
 */
int _mysetenv(shell_info *info)
{
    if (info->argc != 3)
    {
        _eputs("Incorrect number of arguments\n");
        return (1);
    }

    if (setEnvironment(info, info->argv[1], info->argv[2]) == 0)
        return (0);

    return (1);
}

/**
 * _myunsetenv - Remove an environment variable.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *
 * Description: Removes one or more environment variables based on the
 * arguments provided in info->argv.
 *
 * Return: 0 on successful removal of all specified variables, 1 if too few arguments.
 */
int _myunsetenv(shell_info *info)
{
    int i;

    if (info->argc < 2)
    {
        _eputs("Too few arguments.\n");
        return (1);
    }

    for (i = 1; i < info->argc; i++)
    {
        unsetEnvironment(info, info->argv[i]);
    }

    return (0);
}


/**
 * populateEnvList - Populates the environment linked list.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *
 * Description: Iterates over the system environment variables and adds
 * each one to the end of the environment linked list in the info struct.
 *
 * Return: Always returns 0.
 */
int populateEnvList(shell_info *info)
{
    extern char **environ;
    size_t i;

    if (!info)
        return (1);

    info->env = NULL;

    for (i = 0; environ[i]; i++)
    {
        if (append_node(&(info->env), environ[i], 0) == NULL)
        {
            /* Handle error in adding node (optional) */
        }
    }

    return (0);
}

