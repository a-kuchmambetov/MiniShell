#include "builtins.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (!ft_strcmp(cmd, "echo"))
        return (1);
    if (!ft_strcmp(cmd, "cd"))
        return (1);
    if (!ft_strcmp(cmd, "pwd"))
        return (1);
    if (!ft_strcmp(cmd, "export"))
        return (1);
    if (!ft_strcmp(cmd, "unset"))
        return (1);
    if (!ft_strcmp(cmd, "env"))
        return (1);
    if (!ft_strcmp(cmd, "exit"))
        return (1);
    return (0);
}

int exec_builtin(t_shell_data *data, char **args)
{
    if (!ft_strcmp(args[0], "echo"))
        return (builtin_echo(data, args));
    else if (!ft_strcmp(args[0], "cd"))
        return (builtin_cd(data, args));
    else if (!ft_strcmp(args[0], "pwd"))
        return (builtin_pwd(data));
    else if (!ft_strcmp(args[0], "export"))
        return (builtin_export(data, args));
    else if (!ft_strcmp(args[0], "unset"))
        return (builtin_unset(data, args));
    else if (!ft_strcmp(args[0], "env"))
        return (builtin_env(data));
    else if (!ft_strcmp(args[0], "exit"))
        return (builtin_exit(data, args));
    return (1);
}
