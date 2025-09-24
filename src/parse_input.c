#include "../main.h"

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
        data->env.len++;
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