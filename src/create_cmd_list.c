#include "../main.h"
#include "create_cmd_list_utils/create_cmd_list_utils.h"

static int delimiter_type(const char *str)
{
    if (str[0] == '<')
        return (REDIR_IN);
    if (str[0] == '>')
        return (REDIR_OUTPUT);
    if (ft_strncmp(str, ">>", 2) == 0)
        return (REDIR_APPEND);
    if (ft_strncmp(str, "<<", 2) == 0)
        return (REDIR_HEREDOC);
    if (str[0] == '|')
        return (PIPE);
    return (NO_REDIR);
}

static int parsing_str_arr(t_shell_data *dt, t_cmd_node **node,
                           char **str_arr, int *i)
{
    const int delim_type = delimiter_type(str_arr[*i]);

    if (!dt || !str_arr || !node || !i)
        return (1);
    if (delim_type == REDIR_IN || delim_type == REDIR_HEREDOC)
        return (set_input_redir(dt, *node, str_arr, i));
    else if (delim_type == REDIR_OUTPUT || delim_type == REDIR_APPEND)
        return (set_output_redir(*node, str_arr, i));
    else if (delim_type == PIPE)
        return (set_pipe(&dt->cmd_list, node, str_arr, i));
    else
    {
        if (!(*node)->cmd)
            return (set_cmd(*node, str_arr, i));
        else
            return (set_cmd_args(*node, str_arr, i));
    }
    return (0);
}

int trim_args(t_cmd_node *node)
{
    char *trimmed;

    if (!node || !node->args)
        return (0);
    trimmed = ft_strtrim(node->args, " ");
    if (!trimmed)
        return (ft_print_err("Memory allocation error\n"), 1);
    free(node->args);
    node->args = trimmed;
    return (0);
}

int create_cmd_list(t_shell_data *dt, char **str_arr)
{
    t_cmd_node *new_node;
    int i;

    if (!str_arr || !dt)
        return (1);
    new_node = (t_cmd_node *){0};
    if (malloc_cmd_node(&new_node))
        return (ft_print_err("Memory allocation error\n"), 1);
    i = 0;
    while (str_arr[i])
    {
        if (parsing_str_arr(dt, &new_node, str_arr, &i))
        {
            free_cmd_node(new_node);
            return (1);
        }
        i++;
    }
    if (trim_args(new_node))
        return (free_cmd_node(new_node), 1);
    if (new_node && (new_node->cmd || new_node->args || new_node->input_redir || new_node->output_redir))
        push_cmd_node(&dt->cmd_list, new_node);
    else
        free_cmd_node(new_node);
    return (0);
}
