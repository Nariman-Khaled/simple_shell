#include "shell.h"

/**
 * clearInfo - initializes info_t struct
 * @info: struct address
 */
void clearInfo(shell_info *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * setInfo - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void setInfo(shell_info *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias(info);
        replace_shell_variables(info);
	}
}

/**
 * freeInfo - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void freeInfo(shell_info *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
            free_linked_list(&(info->env));
		if (info->history)
            free_linked_list(&(info->history));
		if (info->alias)
            free_linked_list(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
