#include "create_cmd_list_utils.h"

int set_input_redir(t_cmd_node *node, char **str_arr, int *i)
{
    if (!node || !str_arr || !i)
        return (1);
    if (ft_strncmp(str_arr[*i], "<<", 2) == 0)
        node->input_redir_type = REDIR_HEREDOC;
    else
        node->input_redir_type = REDIR_INPUT;
    if (str_arr[*i + 1][0] == ' ')
        *i += 1;
    *i += 1;
    if (node->input_redir_type == REDIR_INPUT && check_file_open(str_arr[*i]))
        return (1);
    if (node->input_redir)
        free(node->input_redir);
    node->input_redir = ft_strdup(str_arr[*i]);
    if (!node->input_redir)
        return (ft_print_err("Memory allocation error\n"), 1);
    return (0);
}

int set_output_redir(t_cmd_node *node, char **str_arr, int *i)
{
    if (!node || !str_arr || !i)
        return (1);
    if (ft_strncmp(str_arr[*i], ">>", 2) == 0)
        node->output_redir_type = REDIR_APPEND;
    else
        node->output_redir_type = REDIR_OUTPUT;
    if (str_arr[*i + 1][0] == ' ')
        *i += 1;
    if (check_file_create(str_arr[*i + 1], node->output_redir_type))
        return (1);
    if (!str_arr[*i + 1])
        return (ft_print_err("syntax error near unexpected token `newline'\n"),
                1);
    *i += 1;
    if (node->output_redir)
        free(node->output_redir);
    node->output_redir = ft_strdup(str_arr[*i]);
    if (!node->output_redir)
        return (ft_print_err("Memory allocation error\n"), 1);
    return (0);
}

int set_pipe(t_cmd_list *cmd_list, t_cmd_node **node,
             char **str_arr, int *i)
{
    if (!node || !str_arr || !i)
        return (1);
    (*node)->is_pipe_out = 1;
    push_cmd_node(cmd_list, *node);
    *node = NULL;
    if (malloc_cmd_node(node))
        return (ft_print_err("Memory allocation error\n"), 1);
    (*node)->is_pipe_in = 1;
    return (0);
}

int set_cmd(t_cmd_node *node, char **str_arr, int *i)
{
    if (!node || !str_arr || !i)
        return (1);
    if (str_arr[*i][0] == ' ')
        return (0);
    if (!node->cmd)
    {
        node->cmd = ft_strdup(str_arr[*i]);
        if (!node->cmd)
            return (ft_print_err("Memory allocation error\n"), 1);
    }
    return (0);
}

int set_cmd_args(t_cmd_node *node, char **str_arr, int *i)
{
    int str_len;
    int arg_len;
    char *temp;

    if (!node || !str_arr || !i)
        return (1);
    str_len = ft_strlen(str_arr[*i]);
    if (node->args)
    {
        arg_len = ft_strlen(node->args);
        temp = ft_calloc(sizeof(char), arg_len + str_len + 1);
        if (!temp)
            return (ft_print_err("Memory allocation error\n"), 1);
        ft_strlcpy(temp, node->args, arg_len + 1);
        ft_strlcat(temp, str_arr[*i], arg_len + str_len + 1);
        free(node->args);
        node->args = temp;
    }
    else
    {
        node->args = ft_strdup(str_arr[*i]);
        if (!node->args)
            return (ft_print_err("Memory allocation error\n"), 1);
    }
    return (0);
}
