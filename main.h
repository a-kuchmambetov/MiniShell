#ifndef MAIN_H
# define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "libft/libft.h"

typedef struct s_env_node
{
    char *key;
    char *value;
    struct s_env_node *next;
} t_env_node;

typedef struct s_env_list
{
    t_env_node *first;
    int size;
} t_env_list;

typedef struct s_shell_data
{
    t_env_list env;
    char **paths;
    int last_exit_status;
} t_shell_data;

void parse_envp(t_shell_data *data, char **envp);
void parse_exec_folders(t_shell_data *data);
char *find_executable(t_shell_data *data, char *command);

void free_env_list(t_env_list *env);
void free_str_arr(char **paths);
void free_shell_data(t_shell_data *data);

#endif