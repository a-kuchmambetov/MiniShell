#include "../main.h"

void parse_exec_folders(t_shell_data *data)
{
    t_env_node *current;

    current = data->env_list.first;
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

int set_envp_from_env(t_shell_data *data)
{
    t_env_node *current;
    int i;

    free_str_arr(data->envp);
    current = data->env_list.first;
    data->envp = ft_calloc(sizeof(char *), (data->env_list.len + 1));
    if (!data->envp)
        return (1);
    i = 0;
    while (i < data->env_list.len)
    {
        data->envp[i] = ft_calloc(sizeof(char), (ft_strlen(current->key) + ft_strlen(current->value) + 2));
        if (!data->envp[i])
            return (1);
        ft_strlcat(data->envp[i], current->key, ft_strlen(current->key) + 1);
        ft_strlcat(data->envp[i], "=", ft_strlen(data->envp[i]) + 2);
        ft_strlcat(data->envp[i], current->value, ft_strlen(data->envp[i]) + ft_strlen(current->value) + 1);
        current = current->next;
        i++;
    }
    return (0);
}

int init_shell_data(t_shell_data *data, char **envp)
{
    char *cwd;

    *data = (t_shell_data){0};
    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (free_shell_data(data), 1);
    if (data->pwd)
        free(data->pwd);
    data->pwd = ft_strdup(cwd);
    if (!data->pwd)
        return (free_shell_data(data), free(cwd), 1);
    free(cwd);
    parse_envp(data, envp);
    parse_exec_folders(data);
    set_envp_from_env(data);
    update_last_exit_status(data, 0);
    return (0);
}
