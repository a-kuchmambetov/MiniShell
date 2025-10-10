#include "../main.h"

void free_env_list(t_env_list *env_list)
{
    t_env_node *current;
    t_env_node *next;

    if (!env_list)
        return;
    current = env_list->first;
    while (current)
    {
        if (!current)
            break;
        next = current->next;
        if (current->key)
            free(current->key);
        if (current->value)
            free(current->value);
        free(current);
        current = next;
    }
}

void free_cmd_node(t_cmd_node *node)
{
    if (!node)
        return;
    if (node->cmd)
        free(node->cmd);
    if (node->args)
        free(node->args);
    if (node->input_redir)
        free(node->input_redir);
    if (node->output_redir)
        free(node->output_redir);
    free(node);
}

void free_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_node *current;
    t_cmd_node *next;

    if (!cmd_list || !cmd_list->first)
        return;
    current = cmd_list->first;
    while (current)
    {
        next = current->next;
        free_cmd_node(current);
        current = next;
    }
    cmd_list = (t_cmd_list *){0};
}

void free_str_arr(char **str_arr)
{
    int i;

    i = 0;
    if (!str_arr)
        return;
    while (str_arr[i])
    {
        if (str_arr[i])
            free(str_arr[i]);
        i++;
    }
    free(str_arr);
}

void free_shell_data(t_shell_data *data)
{
    free_cmd_list(&data->cmd_list);
    free_env_list(&data->env_list);
    free_str_arr(data->paths);
    free_str_arr(data->envp);
    if (data->pwd)
        free(data->pwd);
}
