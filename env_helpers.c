#include "shell.h"
#include <stdlib.h>

/**
 * get_environ - Returns the string array copy of the environment.
 * @info: Structure containing potential arguments and environment list.
 *
 * Description: If the environment has changed, this function updates the
 * string array representation of the environment. It ensures that the
 * environment is always up-to-date when accessed.
 *
 * Return: Pointer to the string array representing the environment.
 */
char **get_environ(shell_info *info)
{
    /* Check if environment needs to be updated */
    if (info->env_changed)
    {
        /* Free the old environment array if it exists */
        if (info->environ)
        {
            free_string_array(info->environ);
        }

        /* Update the environment array */
        info->environ = listToStrings(info->env);
        info->env_changed = 0;
    }

    return info->environ;
}

/**
 * unsetEnvironment - Remove an environment variable.
 * @info: Structure containing potential arguments and the environment list.
 * @var: The string representing the environment variable to remove.
 *
 * Description: Iterates through the environment list and removes the
 * environment variable that matches the provided name.
 *
 * Return: 1 on successful deletion, 0 otherwise.
 */
 
int unsetEnvironment(shell_info *info, char *var)
{
    list_t *node;
    size_t index;
    int env_changed = 0;

    if (!info || !info->env || !var)
        return (0);

    node = info->env;
    index = 0;
    while (node)
    {
        if (starts_with(node->str, var) && node->str[strlen(var)] == '=')
        {
            env_changed = remove_node_at(&(info->env), index);
            node = info->env; /* Reset the node to the start of the list after deletion */
            continue;
        }

        node = node->next;
        index++;
    }

    info->env_changed = env_changed;
    return env_changed;
}

/**
 * setEnvironment - Initialize a new environment variable, or modify an existing one.
 * @info: Structure containing potential arguments and environment list.
 * @var: The string representing the environment variable name.
 * @value: The string representing the environment variable value.
 *
 * Description: Constructs a string in the format 'var=value' and either updates
 * an existing environment variable or adds a new one to the list.
 *
 * Return: 0 on success, 1 on memory allocation failure.
 */
int setEnvironment(shell_info *info, char *var, char *value)
{
    char *new_env_entry;
    list_t *node;
    size_t var_len, value_len;

    if (!var || !value)
        return (0);

    var_len = _strlen(var);
    value_len = _strlen(value);

    new_env_entry = malloc(var_len + value_len + 2);
    if (!new_env_entry)
        return (1);

    _strcpy(new_env_entry, var);
    _strcat(new_env_entry, "=");
    _strcat(new_env_entry, value);

    for (node = info->env; node; node = node->next)
    {
        if (starts_with(node->str, var) && node->str[var_len] == '=')
        {
            free(node->str);
            node->str = new_env_entry;
            info->env_changed = 1;
            return (0);
        }
    }

    append_node(&(info->env), new_env_entry, 0);
    info->env_changed = 1;
    return (0);
}

/**
 * free_string_array - Frees an array of strings.
 * @array: The array of strings to be freed.
 *
 * Description: Iterates through an array of strings, freeing each string,
 * then frees the array itself. Assumes that the array is null-terminated.
 */
void free_string_array(char **array)
{
    int i;

    if (array == NULL)
        return;

    /* Free each string in the array */
    for (i = 0; array[i] != NULL; i++)
    {
        free(array[i]);
    }

    /* Free the array itself */
    free(array);
}
