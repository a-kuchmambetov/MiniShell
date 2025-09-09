#include "main.h"
// compile with: gcc -Wall -Wextra -Werror main.c src/free_utils.c libft.a -o minishell

void parse_envp(t_shell_data *data, char **envp)
{
    t_env_node *new_node;
    t_env_node *current;
    char **res;

    if (!envp || !envp[0])
        return ;
    current = data->env.first;
    for (int i = 0; envp[i] != NULL; i++)
    {
        new_node = malloc(sizeof(t_env_node));
        res = ft_split(envp[i], '=');
        new_node->key = ft_strdup(res[0]);
        new_node->value = ft_strdup(res[1]);
        if (!new_node->key || !new_node->value || !res)
            return (free(new_node->key), free(new_node->value),
                    free(new_node), free_str_arr(res));
        free_str_arr(res);
        new_node->next = NULL;
        if (current)
            current->next = new_node;
        else
            data->env.first = new_node;
        current = new_node;
        data->env.size++;
    }
}

void parse_exec_folders(t_shell_data *data)
{
    t_env_node *current;

    current = data->env.first;
    while (current)
    {
        if (ft_strncmp(current->key, "PATH", 4) == 0)
        {
            data->paths = ft_split(current->value, ':');
            break;
        }
        current = current->next;
    }
}

char *find_executable(t_shell_data *data, char *command)
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
        ft_strlcat(full_path, command, ft_strlen(full_path) + ft_strlen(command) + 1);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        i++;
    }
    return (NULL);
}

void run_command(t_shell_data *data, char *command, char *argv_str)
{
    char *executable;
    pid_t pid;

    executable = find_executable(data, command);
    if (!executable)
    {
        data->last_exit_status = 127 << 8;
        printf("%s: Command not found\n", command);
        if (executable)
            free(executable);
        return ;
    }
    pid = fork();
    if (pid == 0)
    {
        char *args[] = {argv_str, NULL};
        execve(executable, args, NULL);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, &data->last_exit_status, 0);
    }
    free(executable);
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;

    t_shell_data data;
    data = (t_shell_data){0};

    // Check with empty envp
    // char *envp2[] = { "PATH", NULL };
    // parse_envp(&data, envp2);
    // parse_exec_folders(&data);

    parse_envp(&data, envp);
    parse_exec_folders(&data);

    // t_env_node *current;
    // current = data.env.first;
    // int i = 0;
    // while (i < data.env.size)
    // {
    //     printf("Key and value %s=%s\n", current->key, current->value);
    //     current = current->next;
    //     i++;
    // }

    // printf("\nPATHS:\n");
    // i = 0;
    // while (data.paths[i])
    // {
    //     printf("Path %s\n", data.paths[i]);
    //     i++;
    // }

    printf("\nSEARCHING EXECUTABLE:\n");

    printf("Looking for ls: ");
    char *result = find_executable(&data, "ls");
    if (result == NULL)
        printf("Command not found\n");
    else
    {
        printf("\nCommand FOUND: %s\nRunning...\n\n", result);
        run_command(&data, "ls", "-la");
        printf("\nCommand finished.\n");
        printf("Last exit status: %d\n\n", WEXITSTATUS(data.last_exit_status));
    }
    free(result);

    printf("Looking for gcc: ");
    result = find_executable(&data, "gcc");
    if (result == NULL)
        printf("Command not found\n");
    else
        printf("%s\n\n", result);
    free(result);

    printf("Looking for nonexistentcommand: ");
    result = find_executable(&data, "nonexistentcommand");
    if (result == NULL)
        printf("Command not found\n");
    else
        printf("%s\n\n", result);
    run_command(&data, "lss", "-la");

    printf("Last exit status: %d\n", WEXITSTATUS(data.last_exit_status));
    if (result)
        free(result);
    free_shell_data(&data);
    return 0;
}