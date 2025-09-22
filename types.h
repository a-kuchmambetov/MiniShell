#ifndef TYPES_H
#define TYPES_H

typedef struct s_cmd_node
{
    char *cmd;
    char *args;
    char *input_redir;
    int input_type; // 2: <<, 1: <, 0: |, -1: normal command
    char *output_redir;
    int output_type; // 2: >>, 1: >, 0: |, -1: normal command
    struct s_cmd_node *prev;
    struct s_cmd_node *next;
} t_cmd_node;

typedef struct s_cmd_list
{
    t_cmd_node *first;
    int len;
} t_cmd_list;

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
    t_cmd_list cmd_list;
    // Environment variables and execution paths
    t_env_list env_list;
    char **paths;
    char **envp;
    char *pwd;
    // File descriptors for input/output redirection and pipes
    int fd_in;
    int fd_out;
    int pipefd1[2];
    // Last command exit status
    int last_exit_status;
} t_shell_data;

typedef struct s_split_data
{
    char **arr;
    int l;
    int i;
    int row;
} t_split_data;

#endif // TYPES_H