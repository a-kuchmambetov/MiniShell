#include "executor.h"

char **build_argv(t_cmd_node *cmd)
{
    char **argv;

    if (!cmd || !cmd->cmd)
        return (NULL);
    argv = ft_calloc(sizeof(char *), 3);
    if (!argv)
        return (NULL);
    argv[0] = ft_strdup(cmd->cmd);
    if (cmd->args)
        argv[1] = ft_strdup(cmd->args);
    else
        argv[1] = NULL;
    if (!argv[0] || (!argv[1] && cmd->args))
    {
        free_str_arr(argv);
        return (NULL);
    }
    return (argv);
}
