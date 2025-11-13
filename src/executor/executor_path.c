#include "executor.h"
#include <errno.h>

static void exit_exec_failure(char **argv, char *path)
{
    int code;

    code = errno == EACCES ? 126 : 127;
    perror(argv[0]);
    if (path)
        free(path);
    free_str_arr(argv);
    exit(code);
}

char *resolve_command_path(t_shell_data *data, const char *command)
{
    char    *full;
    size_t  cmd_len;
    size_t  dir_len;
    int     i;

    if (!data->paths || !command)
        return (NULL);
    cmd_len = ft_strlen(command);
    i = 0;
    while (data->paths[i])
    {
        dir_len = ft_strlen(data->paths[i]);
        full = malloc(dir_len + cmd_len + 2);
        if (!full)
            return (NULL);
        ft_strlcpy(full, data->paths[i], dir_len + 1);
        ft_strlcat(full, "/", dir_len + 2);
        ft_strlcat(full, command, dir_len + cmd_len + 2);
        if (access(full, X_OK) == 0)
            return (full);
        free(full);
        i++;
    }
    return (NULL);
}

void exec_external(t_shell_data *data, char **argv)
{
    char *path;

    if (!argv || !argv[0])
        exit(0);
    if (ft_strchr(argv[0], '/'))
    {
        execve(argv[0], argv, data->envp);
        exit_exec_failure(argv, NULL);
    }
    path = resolve_command_path(data, argv[0]);
    if (!path)
    {
        ft_print_err("%s: command not found\n", argv[0]);
        free_str_arr(argv);
        exit(127);
    }
    execve(path, argv, data->envp);
    exit_exec_failure(argv, path);
}
