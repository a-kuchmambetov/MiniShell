#include "../main.h"

static char *find_executable(t_shell_data *data, char *command)
{
    int i;
    char *full_path;

    i = 0;
    if (!data->paths)
        return (NULL);
    while (data->paths[i])
    {
        full_path = malloc(ft_strlen(data->paths[i]) + ft_strlen(command) + 2);
        if (!full_path)
            return (NULL);
        ft_strlcpy(full_path, data->paths[i], ft_strlen(data->paths[i]) + 1);
        ft_strlcat(full_path, "/", ft_strlen(full_path) + 2);
        ft_strlcat(full_path, command, ft_strlen(full_path)
            + ft_strlen(command) + 1);
        printf("Checking path: %s\n", full_path);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        i++;
    }
    return (NULL);
}

void exec_cmd(t_shell_data *data, char *command, char **argv_str)
{
    char *executable;
    pid_t pid;

    executable = find_executable(data, command);
    if (!executable)
    {
        data->last_exit_status = 127 << 8;
        ft_print_err("%s: command not found\n", command);
        return ;
    }
    pid = fork();
    if (pid == 0)
    {
        execve(executable, argv_str, data->envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
        waitpid(pid, &data->last_exit_status, 0);
    free(executable);
}
