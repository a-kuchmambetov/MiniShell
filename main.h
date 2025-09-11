#ifndef MAIN_H
# define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

// ANSI color codes
# define COLOR_RESET    "\033[0m"
# define COLOR_RED      "\033[31m"
# define COLOR_GREEN    "\033[32m"
# define COLOR_YELLOW   "\033[33m"
# define COLOR_BLUE     "\033[34m"
# define COLOR_MAGENTA  "\033[35m"
# define COLOR_CYAN     "\033[36m"
# define COLOR_WHITE    "\033[37m"
# define COLOR_BOLD     "\033[1m"
# define COLOR_DIM      "\033[2m"

typedef struct s_env_node
{
    char *key;
    char *value;
    struct s_env_node *next;
} t_env_node;

typedef struct s_env_list
{
    t_env_node *first;
    int len;
} t_env_list;

typedef struct s_shell_data
{
    t_env_list env;
    char **paths;
    char **envp;
    char *pwd;
    // File descriptors for input/output redirection and pipes
    int fd_in;
    int fd_out;
    int pipefd1[2];
    int pipefd2[2];
    // Last command exit status
    int last_exit_status;
} t_shell_data;

void parse_envp(t_shell_data *data, char **envp);
void parse_exec_folders(t_shell_data *data);

void free_env_list(t_env_list *env);
void free_str_arr(char **paths);
void free_shell_data(t_shell_data *data);

void set_envp_from_env(t_shell_data *data);
void init_shell_data(t_shell_data *data, char **envp);

void exec_cmd(t_shell_data *data, char *command, char *argv_str);
void exec_cmd_2(t_shell_data *data, char *command, char **str_arr);

int ft_print_err(const char *format, ...);

char *read_input();
void process_input(t_shell_data *data, char *input);

// Tokenizer: splits by < << > >> | and includes delimiters
char **split_by_delims(const char *s);

void print_prompt_header(void);

#endif
