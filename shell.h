#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE	".shell_history"
#define HISTORY_MAX	4096

extern char **environ;

/**
 * struct list - single linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct list
{
	int num;
	char *str;
	struct list *next;
} list_t;

/**
 *struct shellInfo - contains arguements to pass
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct shellInfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} shell_info;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}


/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3


/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(shell_info *);
} builtin_table;



/* builtin_emulators.c */
int _myexit(shell_info *);
int _mycd(shell_info *);
int _myhelp(shell_info *);

/* builtin_emulators2.c */
int _myhistory(shell_info *);
int _myalias(shell_info *);
/* hsh.c */
int exec_shell(shell_info *, char **);
int find_builtin(shell_info *);
void find_cmd(shell_info *);
void fork_cmd(shell_info *);


/* path.c */
int is_cmd(shell_info *, char *);
char *dup_chars(char *, int, int);
char *find_path(shell_info *, char *, char *);


void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory_functions */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int bfree(void **);
/* more_functions.c */
int interactive(shell_info *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* more_functions2.c */
int _erratoi(char *);
void print_error(shell_info *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* getline.c module */
ssize_t get_input(shell_info *);
int _getline(shell_info *, char **, size_t *);
void sigintHandler(int);

/* info.c module */
void clear_info(shell_info *);
void set_info(shell_info *, char **);
void free_info(shell_info *, int);

/* env.c module */
char *_getenv(shell_info *, const char *);
int _myenv(shell_info *);
int _mysetenv(shell_info *);
int _myunsetenv(shell_info *);
int populate_env_list(shell_info *);

/* env2.c module */
char **get_environ(shell_info *);
int _unsetenv(shell_info *, char *);
int _setenv(shell_info *, char *, char *);

/* file_io_functions.c */
char *get_history_file(shell_info *info);
int write_history(shell_info *info);
int read_history(shell_info *info);
int build_history_list(shell_info *info, char *buf, int linecount);
int renumber_history(shell_info *info);

/* liststr.c module */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* liststr2.c module */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);
/* chain.c */
int is_chain(shell_info *, char *, size_t *);
void check_chain(shell_info *, char *, size_t *, size_t, size_t);
int replace_alias(shell_info *);
int replace_vars(shell_info *);
int replace_string(char **, char *);

#endif
