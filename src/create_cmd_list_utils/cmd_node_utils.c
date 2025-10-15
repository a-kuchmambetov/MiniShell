#include "create_cmd_list_utils.h"

void push_cmd_node(t_cmd_list *cmd_list, t_cmd_node *new_node)
{
    t_cmd_node *current;

    if (!cmd_list || !new_node)
        return;
    current = cmd_list->first;
    if (!current)
    {
        cmd_list->first = new_node;
        cmd_list->len = 1;
        return;
    }
    while (current->next)
        current = current->next;
    current->next = new_node;
    cmd_list->len++;
}

int malloc_cmd_node(t_cmd_node **new_node)
{
    t_cmd_node *node;

    node = malloc(sizeof(t_cmd_node));
    if (!node)
        return (1);
    node->cmd = NULL;
    node->args = NULL;
    node->input_redir_type = NO_REDIR;
    node->input_redir = NULL;
    node->output_redir_type = NO_REDIR;
    node->output_redir = NULL;
    node->is_pipe_in = 0;
    node->is_pipe_out = 0;
    node->next = NULL;
    *new_node = node;
    return (0);
}

int check_args_expansion(t_shell_data *dt, t_cmd_node *node, char *args, char *str)
{
    const int args_len = ft_strlen(args);
    char *temp;
    char *res;

    temp = ft_strdup(str);
    if (!temp)
        return (ft_print_err("Memory allocation error\n"), 1);
    if (check_do_expansion(dt->env_list, &temp))
        return (ft_print_err("error during argument expansion\n"),
                free(temp), 1);
    res = ft_calloc(sizeof(char), args_len + ft_strlen(temp) + 1);
    if (!res)
        return (ft_print_err("Memory allocation error\n"), free(temp), 1);
    ft_strlcat(res, args, args_len + 1);
    ft_strlcat(res, temp, args_len + ft_strlen(temp) + 1);
    free(temp);
    if (node->args)
        free(node->args);
    node->args = res;
    if (!node->args)
        return (ft_print_err("Memory allocation error\n"), 1);
    return (0);
}
