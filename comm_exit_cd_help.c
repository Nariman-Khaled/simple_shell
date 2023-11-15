#include "shell.h"

/**
 * exitShell - Exits the shell with a specified status.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *
 * Description: Exits the shell with the status provided in info->argv[1].
 * If info->argv[1] is not a valid number, an error message is displayed,
 * and the function returns without exiting.
 *
 * Return: 1 if there is an error in exit argument, -2 to indicate shell exit.
 */
int exitShell(shell_info *info)
{
    int exit_status;

    /* Check if an exit argument is provided */
    if (info->argv[1])
    {
        exit_status = _erratoi(info->argv[1]);

        /* Validate the exit argument */
        if (exit_status == -1)
        {
            info->status = 2;
            print_error(info, "Illegal number: ");
            _eputs(info->argv[1]);
            _buffered_err_putchar('\n');
            return (1);
        }

        info->err_num = exit_status;
    }
    else
    {
        info->err_num = -1;
    }

    return (-2);
}

/**
 * shell_cd - Changes the current directory of the process.
 * @info: Structure containing potential arguments. Used to maintain
 *        a constant function prototype.
 *
 * Description: Changes the working directory based on the arguments in info->argv.
 * Defaults to the HOME directory, or OLDPWD if '-' is provided.
 *
 * Return: Always returns 0.
 */
int shell_cd(shell_info *info)
{
    char buffer[1024], *dir = NULL;

    if (!getcwd(buffer, sizeof(buffer)))
    {
        _puts("Error: Unable to get current working directory.\n");
        return (1);
    }

    if (!info->argv[1] || _strcmp(info->argv[1], "-") == 0)
    {
        dir = getEnvironment(info, !info->argv[1] ? "HOME=" : "OLDPWD=");
        if (!dir)
            dir = !info->argv[1] ? "/" : buffer;
    }
    else
    {
        dir = info->argv[1];
    }

    if (chdir(dir) == -1)
    {
        print_error(info, "can't cd to ");
        _eputs(info->argv[1]), _buffered_err_putchar('\n');
    }
    else
    {
        setEnvironment(info, "OLDPWD", getEnvironment(info, "PWD="));
        getcwd(buffer, sizeof(buffer));
        setEnvironment(info, "PWD", buffer);
        if (_strcmp(info->argv[1], "-") == 0)
            _puts(buffer), _putchar('\n');
    }
    return (0);
}



/**
 * shell_help - Displays help information for shell commands.
 * @info: Structure containing potential arguments.
 *
 * Description: Currently, this function is a placeholder for the help command.
 * It will be expanded to display help information based on the arguments provided.
 *
 * Return: Always returns 0.
 */
int shell_help(shell_info *info)
{
    /* Placeholder for future implementation */
    _puts("Help functionality is under development.\n");

    /* Example of how to handle arguments when implemented */
    if (info->argv[1])
    {
        _puts("Help for command: ");
        _puts(info->argv[1]);
        _puts(" is not yet implemented.\n");
    }

    return (0);
}

