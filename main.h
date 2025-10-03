#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

// Project custom headers
#include "types.h"
#include "libft/libft.h"

// ANSI color codes
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"
#define COLOR_DIM "\033[2m"

void parse_envp(t_shell_data *data, char **envp);
void parse_exec_folders(t_shell_data *data);

void free_env_list(t_env_list *env);
void free_str_arr(char **paths);
void free_shell_data(t_shell_data *data);

int set_envp_from_env(t_shell_data *data);
void init_shell_data(t_shell_data *data, char **envp);

void exec_cmd(t_shell_data *data, char *command, char *argv_str);

int ft_print_err(const char *format, ...);

char *read_input();
void process_input(t_shell_data *data, char *input);

void print_prompt_header(void);
/// Built-in commands
int is_builtin(char *cmd);
int exec_builtin(t_shell_data *data, char **args);

// Command t_cmd_list list management
void push_cmd_node(t_cmd_list *cmd_list, t_cmd_node *new_node);
char **split_input_str(const char *s);
void print_prompt_header(void);
int create_cmd_list(t_cmd_list *cmd_list, char **str_arr);

#endif
