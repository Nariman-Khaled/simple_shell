#include "shell.h"

/**
 * execShell - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int execShell(shell_info *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
        clearInfo(info);
		if (interactive(info))
			_puts("$ ");
        _buffered_err_putchar(BUF_FLUSH);
		r = retrieve_input_line(info);
		if (r != -1)
		{
            setInfo(info, av);
			builtin_ret = findBuiltin(info);
			if (builtin_ret == -1)
                findCmd(info);
		}
		else if (interactive(info))
			_putchar('\n');
        freeInfo(info, 0);
	}
    save_command_history(info);
    freeInfo(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * findBuiltin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int findBuiltin(shell_info *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit",     exitShell},
		{"env",      shellEnvironment},
		{"help",     shell_help},
		{"history",  shell_history},
		{"setenv",   _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd",       shell_cd},
		{"alias",    alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * findCmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void findCmd(shell_info *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = findPath(info, getEnvironment(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
        forkCmd(info);
	}
	else
	{
		if ((interactive(info) || getEnvironment(info, "PATH=")
             || info->argv[0][0] == '/') && isCmd(info, info->argv[0]))
            forkCmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * forkCmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void forkCmd(shell_info *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
            freeInfo(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}
