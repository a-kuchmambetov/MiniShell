#include "../main.h"

static void free_node(t_env_node *node)
{
    if (!node)
        return;
    if (node->key)
        free(node->key);
    if (node->value)
        free(node->value);
    free(node);
}

static void abort_parse_envp(t_shell_data *data,
                             t_env_node *new_node,
                             char **res)
{
    free_node(new_node);
    free_str_arr(res);
    free_env_list(&data->env_list);
    data->env_list.first = NULL;
    data->env_list.len = 0;
}

void parse_envp(t_shell_data *data, char **envp)
{
    t_env_node *new_node;
    t_env_node *current;
    char **res;
    int i;

    current = data->env_list.first;
    i = -1;
    while (envp[++i] != NULL)
    {
        new_node = ft_calloc(sizeof(t_env_node), 1);
        res = ft_split(envp[i], '=');
        if (!new_node || !res)
            return (abort_parse_envp(data, new_node, res));
        new_node->key = ft_strdup(res[0]);
        new_node->value = ft_strdup(res[1]);
        if (!new_node->key || !new_node->value)
            return (abort_parse_envp(data, new_node, res));
        free_str_arr(res);
        if (current)
            current->next = new_node;
        else
            data->env_list.first = new_node;
        current = new_node;
        data->env_list.len++;
    }
}

int parse_exec_folders(t_shell_data *data)
{
    t_env_node *current = data->env_list.first;
    char **paths;

    while (current)
    {
        if (ft_strncmp(current->key, "PATH", 4) == 0)
        {
            paths = ft_split(current->value, ':');
            if (!paths)
                return (1);
            free_str_arr(data->paths);
            data->paths = paths;
            return (0);
        }
        current = current->next;
    }
    return (0);
}

static void free_partial_envp(char **envp, int count)
{
    while (count-- > 0)
        free(envp[count]);
    free(envp);
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
        data->envp[i] = ft_calloc(sizeof(char), ft_strlen(current->key) + ft_strlen(current->value) + 2);
        if (!data->envp[i])
            return (free_partial_envp(data->envp, i), data->envp = NULL, 1);
        ft_strlcat(data->envp[i], current->key, ft_strlen(current->key) + 1);
        ft_strlcat(data->envp[i], "=", ft_strlen(data->envp[i]) + 2);
        ft_strlcat(data->envp[i], current->value, ft_strlen(data->envp[i]) + ft_strlen(current->value) + 1);
        current = current->next;
        i++;
    }
    return (0);
}

void init_shell_data(t_shell_data *data, char **envp)
{
    char *cwd;

    *data = (t_shell_data){0};
    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (free_shell_data(data), (void)0);
    if (data->pwd)
        free(data->pwd);
    data->pwd = ft_strdup(cwd);
    free(cwd);
    parse_envp(data, envp);
    parse_exec_folders(data);
    set_envp_from_env(data);
}
