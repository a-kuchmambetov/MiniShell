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

static char **split_by_equal(char *env_var)
{
    char **res;
    int i;

    res = NULL;
    i = 0;
    while (env_var[i] && env_var[i] != '=')
        i++;
    if (env_var[i] == '=')
    {
        res = ft_calloc(3, sizeof(char *));
        if (!res)
            return (NULL);
        res[0] = ft_substr(env_var, 0, i);
        res[1] = ft_strdup(env_var + i + 1);
        if (!res || !res[0] || !res[1])
            return (my_free(res[0]), my_free(res[1]), my_free(res), NULL);
        return (res);
    }
    res = ft_calloc(2, sizeof(char *));
    if (!res)
        return (NULL);
    res[0] = ft_strdup(env_var);
    if (!res[0])
        return (free(res[0]), free(res), NULL);
    return (res);
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
        res = split_by_equal(envp[data->env_list.len]);
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