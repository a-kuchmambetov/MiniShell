#include "executor.h"

static size_t arg_count(char **extra)
{
    size_t len;

    len = 0;
    if (!extra)
        return (0);
    while (extra[len])
        len++;
    return (len);
}

static int copy_extra(char **dst, char **extra)
{
    size_t i;

    if (!extra)
        return (0);
    i = 0;
    while (extra[i])
    {
        dst[i] = ft_strdup(extra[i]);
        if (!dst[i])
            return (1);
        i++;
    }
    return (0);
}

char **build_argv(t_cmd_node *cmd)
{
    char    **argv;
    char    **extra;
    size_t  len;
    int     errored;

    if (!cmd || !cmd->cmd)
        return (NULL);
    errored = 0;
    extra = split_args(cmd->args, &errored);
    if (errored)
        return (NULL);
    len = arg_count(extra);
    argv = ft_calloc(sizeof(char *), len + 2);
    if (!argv)
        return (free_str_arr(extra), NULL);
    argv[0] = ft_strdup(cmd->cmd);
    if (!argv[0] || copy_extra(argv + 1, extra))
    {
        free_str_arr(argv);
        free_str_arr(extra);
        return (NULL);
    }
    free_str_arr(extra);
    return (argv);
}
