#include "builtins.h"

int is_builtin(char *cmd)
{
    if (!cmd || !cmd[0])
        return (0);
    if (!ft_strncmp(cmd, "echo", 5))
        return (1);
    if (!ft_strncmp(cmd, "cd", 3))
        return (1);
    if (!ft_strncmp(cmd, "pwd", 4))
        return (1);
    if (!ft_strncmp(cmd, "export", 7))
        return (1);
    if (!ft_strncmp(cmd, "unset", 6))
        return (1);
    if (!ft_strncmp(cmd, "env", 4))
        return (1);
    if (!ft_strncmp(cmd, "exit", 5))
        return (1);
    return (0);
}

int exec_builtin(t_shell_data *data, char **args)
{
    if (!ft_strncmp(args[0], "echo", 5))
        return (builtin_echo(args, data->envp));
    // else if (!ft_strcmp(args[0], "cd"))
    //     return (builtin_cd(data, args));
    // else if (!ft_strcmp(args[0], "pwd"))
    //     return (builtin_pwd(data));
    else if (!ft_strncmp(args[0], "export", 7))
        return (builtin_export(data, args));
    // else if (!ft_strcmp(args[0], "unset"))
    //     return (builtin_unset(data, args));
    // else if (!ft_strcmp(args[0], "env"))
    //     return (builtin_env(data));
    // else if (!ft_strcmp(args[0], "exit"))
    //     return (builtin_exit(data, args));
    return (1);
}
