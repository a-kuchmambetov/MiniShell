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

static int create_new(t_shell_data *dt, char *code_str)
{
    t_env_node *current;
    t_env_node *new_node;
    int i;

    i = 0;
    current = dt->env_list.first;
    while (current->next)
    {
        current = current->next;
        i++;
    }
    new_node = ft_calloc((sizeof(t_env_node)), 1);
    if (!new_node)
        return (1);
    new_node->key = ft_strdup("?");
    if (!new_node->key)
        return (free_env_node(new_node), 1);
    new_node->value = code_str;
    if (current)
        current->next = new_node;
    else
        dt->env_list.first = new_node;
    dt->env_list.len++;
    return (0);
}

int update_last_exit_status(t_shell_data *dt, int last_cmd_code)
{
    t_env_node *current;
    char *code_str;

    if (!dt)
        return (1);
    dt->last_exit_status = last_cmd_code;
    current = dt->env_list.first;
    code_str = ft_itoa(last_cmd_code);
    if (!code_str)
        return (1);
    while (current)
    {
        if (ft_strncmp(current->key, "?", 1) == 0)
        {
            my_free(current->value);
            current->value = code_str;
            if (!current->value)
                return (1);
            return (0);
        }
        current = current->next;
    }
    return (create_new(dt, code_str));
}
