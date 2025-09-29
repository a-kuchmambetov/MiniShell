#ifndef BUILTINS_H
# define BUILTINS_H

#include "../../main.h"

typedef struct s_parser
{
    const char *s;
    char        *res;
    int         pos;
    int         bufsize;
    char        **envp;
    int         i;
} t_parser;


int	builtin_echo(char **argv, char **envp);
int builtin_cd(t_shell_data *data, char **args);
int	builtin_pwd(t_shell_data *data, char **args);
int builtin_export(t_shell_data *data, char **args);
// int builtin_unset(t_shell_data *data, char **args);
// int builtin_env(t_shell_data *data);
// int builtin_exit(t_shell_data *data, char **args);

int is_builtin(char *cmd);
int exec_builtin(t_shell_data *data, char **args);
char	*get_env_value(char **envp, const char *name);
char *ft_strjoin_free(char *s1, const char *sep, const char *s2);
int	update_env(char **envp, const char *name, const char *value);
int add_or_update_env(t_shell_data *data, const char *arg);
int sync_envp(t_shell_data *data);

#endif

