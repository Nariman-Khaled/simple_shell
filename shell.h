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

#define WRITE_BUF_SIZE 1024
#define NULL_TERMINATOR '\0'


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
int exitShell(shell_info *info);
int shell_cd(shell_info *info);
int shell_help(shell_info *info);

/* builtin_emulators2.c */
int shell_history(shell_info *info);
int alias(shell_info *info);
/* hsh.c */
int execShell(shell_info *info, char **av);
int findBuiltin(shell_info *info);
void findCmd(shell_info *info);
void forkCmd(shell_info *info);


/* path.c */
int isCmd(shell_info *info, char *path);
char *dupChars(const char *, int, int);
char *findPath(shell_info *info, char *pathstr, char *cmd);


void _eputs(char *);
int _buffered_err_putchar(char);
int _buffered_fd_putchar(char c, int fd);
int _fd_puts(char *str, int fd);

int _strlen(const char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

char *_strcpy(char *, const char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

char *_strncpy(char *, const char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory_functions */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *reAllocate(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int bfree(void **);
/* more_functions.c */
int interactive(shell_info *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(const char *);

/* more_functions2.c */
int _erratoi(char *);
void print_error(shell_info *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* getline.c module */
ssize_t retrieve_input_line(shell_info *info);
int custom_getline(shell_info *info, char **ptr, size_t *length);
void handle_ctrl_c_signal(int);

/* info.c module */
void clearInfo(shell_info *info);
void setInfo(shell_info *info, char **av);
void freeInfo(shell_info *info, int all);

/* env.c module */
char *getEnvironment(shell_info *info, const char *name);
int shellEnvironment(shell_info *info);
int _mysetenv(shell_info *);
int _myunsetenv(shell_info *);
int populateEnvList(shell_info *info);

/* env2.c module */
char **get_environ(shell_info *);
int unsetEnvironment(shell_info *info, char *var);
int setEnvironment(shell_info *info, char *var, char *value);

/* file_io_functions.c */
char *getHistoryFile(shell_info *info);
int save_command_history(shell_info *info);
int load_command_history(shell_info *info);
int append_to_history(shell_info *info, char *buf, int linecount);
int renumberHistory(shell_info *info);

/* liststr.c module */
list_t *prepend_node(list_t **head, const char *str, int num);
list_t *append_node(list_t **head, const char *str, int num);
size_t printStringList(const list_t *);
int remove_node_at(list_t **head, unsigned int index);
void free_linked_list(list_t **head_ptr);

/* liststr2.c module */
size_t listLen(const list_t *h);
char **listToStrings(list_t *head);
size_t printList(const list_t *h);
list_t *nodeStartsWith(list_t *node, char *prefix, char c);
ssize_t get_node_index(list_t *, list_t *);
/* chain.c */
int checkCommandChain(shell_info *info, char *buf, size_t *p);
void check_chain(shell_info *, char *, size_t *, size_t, size_t);
int replace_alias(shell_info *);
int replace_shell_variables(shell_info *info);
int replace_string(char **, char *);

void free_string_array(char **array);

void prune_history(shell_info *info, int max_entries);
#endif
