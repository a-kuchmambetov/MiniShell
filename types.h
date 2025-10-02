#ifndef TYPES_H
#define TYPES_H

typedef enum e_quote
{
    NO_QUOTE = 0,
    YES_QUOTE = 1
} t_quote;

typedef enum e_redir
{
    NO_REDIR = 0,
    REDIR_INPUT = 1,
    REDIR_OUTPUT = 2,
    REDIR_APPEND = 3,
    REDIR_HEREDOC = 4,
    REDIR_PIPE = 5
} t_redir;

typedef struct s_cmd_node
{
    char *cmd;
    char *args;
    t_redir redir_input;
    char *input_redir;
    t_redir redir_output;
    char *output_redir;
    int is_pipe_in;
    int is_pipe_out;
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
    int pipefd[2];
    // Last command exit status
    int last_exit_status;
} t_shell_data;

typedef struct s_split_data
{
    char **arr;
    int l;
    int i;
    int row;
    t_quote in_quote;
} t_split_data;

#endif // TYPES_H