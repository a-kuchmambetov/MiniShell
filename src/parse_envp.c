#include "../main.h"

static void free_env_node(t_env_node *node)
{
    if (!node)
        return;
    if (node->key)
        free(node->key);
    if (node->value)
        free(node->value);
    free(node);
}
static int set_new_env_node(t_env_node *node, char **res)
{
    node->key = ft_strdup(res[0]);
    if (!res[1])
        node->value = ft_strdup("");
    else
        node->value = ft_strdup(res[1]);
    if (!node->key || !node->value)
        return (1);
    return (0);
}
void parse_envp(t_shell_data *data, char **envp)
{
    t_env_node *new_node;
    t_env_node *current;
    char **res;

    if (!envp || !envp[0] || !*envp[0])
        return;
    current = data->env_list.first;
    while (envp[data->env_list.len])
    {
        new_node = ft_calloc(sizeof(t_env_node), 1);
        res = ft_split(envp[data->env_list.len], '=');
        if (!new_node || !res)
            return (free_env_node(new_node), free_str_arr(res));
        if (set_new_env_node(new_node, res))
            return (free_env_node(new_node), free_str_arr(res));
        free_str_arr(res);
        if (current)
            current->next = new_node;
        else
            data->env_list.first = new_node;
        current = new_node;
        data->env_list.len++;
    }
}