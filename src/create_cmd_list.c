#include "../main.h"

static int is_delimiter(const char *str)
{
    if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
        return (2);
    if (str[0] == '|' || str[0] == '>' || str[0] == '<')
        return (1);
    return (0);
}

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
    new_node->prev = current;
    current->next = new_node;
    cmd_list->len++;
}

int create_cmd_list(t_cmd_list *cmd_list, char **str_arr)
{
    t_cmd_node *new_node;
    int i;

    if (!str_arr || !cmd_list)
        return (1);
    i = 0;
    while (str_arr[i])
    {
        if (is_delimiter(str_arr[i]) == 0)
        {
            new_node = (t_cmd_node *)malloc(sizeof(t_cmd_node));
            if (!new_node)
                return (ft_print_err("Memory allocation error\n"), 1);
            new_node->cmd = ft_strdup(str_arr[i]);
            new_node->args = NULL;
            new_node->prev = NULL;
            new_node->next = NULL;
            push_cmd_node(cmd_list, new_node);
        }
        else if (is_delimiter(str_arr[i]) > 0)
        {
            ft_print_err("Syntax error near unexpected token '%s'\n", str_arr[i]);
            return (1);
        }
        i++;
    }
    return (0);
}
