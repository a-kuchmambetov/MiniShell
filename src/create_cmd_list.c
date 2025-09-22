#include "../main.h"

static int is_delimiter(const char *str)
{
    if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
        return (2);
    if (str[0] == '|' || str[0] == '>' || str[0] == '<')
        return (1);
    return (0);
}

int create_cmd_list(t_cmd_list *cmd_list, char **str_arr)
{
    int i;
    t_cmd_node *new_node;

    if (!str_arr || !cmd_list)
        return (1);
    i = 0;
    while (str_arr[i])
    {
        new_node = malloc(sizeof(t_cmd_node));
        if (!new_node)
            return (ft_print_err("Error: cmd malloc failed\n"), 1);
        new_node->cmd = ft_strdup(str_arr[i]);
        if (!new_node->cmd)
            return (free(new_node), ft_print_err("Error: cmd strdup failed\n"), 1);
        new_node->next = NULL;
        push_cmd_node(cmd_list, new_node);
        i++;
    }
    return (0);
}