#include "../main.h"

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

    node = (t_cmd_node *)malloc(sizeof(t_cmd_node));
    if (!node)
        return (1);
    node->cmd = NULL;
    node->args = NULL;
    node->redir_input = NO_REDIR;
    node->input_redir = NULL;
    node->redir_output = NO_REDIR;
    node->output_redir = NULL;
    node->is_pipe_in = 0;
    node->is_pipe_out = 0;
    node->next = NULL;
    *new_node = node;
    return (0);
}

static int delimiter_type(const char *str)
{
    if (str[0] == '<')
        return (REDIR_INPUT);
    if (str[0] == '>')
        return (REDIR_OUTPUT);
    if (ft_strncmp(str, ">>", 2) == 0)
        return (REDIR_APPEND);
    if (ft_strncmp(str, "<<", 2) == 0)
        return (REDIR_HEREDOC);
    if (str[0] == '|')
        return (REDIR_PIPE);
    return (NO_REDIR);
}

int process_input_redir(t_cmd_node *node, char **str_arr, int *i)
{
    if (!node || !str_arr || !i)
        return (1);
    if (node->input_redir)
        free(node->input_redir);
    if (ft_strncmp(str_arr[*i], "<<", 2) == 0)
        node->redir_input = REDIR_HEREDOC;
    else
        node->redir_input = REDIR_INPUT;
    if (str_arr[*i + 1][0] == ' ')
        *i += 1;
    if (!str_arr[*i + 1])
        return (ft_print_err("Syntax error near unexpected token `newline'\n"), 1);
    *i += 1;
    node->input_redir = ft_strdup(str_arr[*i]);
    if (!node->input_redir)
        return (ft_print_err("Memory allocation error\n"), 1);
    return (0);
}

int process_output_redir(t_cmd_node *node, char **str_arr, int *i)
{
    if (!node || !str_arr || !i)
        return (1);
    if (node->output_redir)
        free(node->output_redir);
    if (ft_strncmp(str_arr[*i], ">>", 2) == 0)
        node->redir_output = REDIR_APPEND;
    else
        node->redir_output = REDIR_OUTPUT;
    if (str_arr[*i + 1][0] == ' ')
        *i += 1;
    if (!str_arr[*i + 1])
        return (ft_print_err("Syntax error near unexpected token `newline'\n"), 1);
    *i += 1;
    node->output_redir = ft_strdup(str_arr[*i]);
    if (!node->output_redir)
        return (ft_print_err("Memory allocation error\n"), 1);
    return (0);
}

int process_pipe(t_cmd_list *cmd_list, t_cmd_node **node, char **str_arr, int *i)
{
    if (!node || !str_arr || !i)
        return (1);
    (*node)->is_pipe_out = 1;
    push_cmd_node(cmd_list, *node);
    if (malloc_cmd_node(node))
        return (ft_print_err("Memory allocation error\n"), 1);
    (*node)->is_pipe_in = 1;
    return (0);
}

int process_cmd(t_cmd_node *node, char **str_arr, int *i)
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

int process_cmd_args(t_cmd_node *node, char **str_arr, int *i)
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
        node->args = ft_strdup(str_arr[*i]);
    if (!node->args)
        return (ft_print_err("Memory allocation error\n"), 1);
    return (0);
}

static int process_str_arr(t_cmd_list *cmd_list, t_cmd_node **node, char **str_arr, int *i)
{
    const int delim_type = delimiter_type(str_arr[*i]);

    if (!cmd_list || !str_arr || !node || !i)
        return (1);
    if (delim_type == REDIR_INPUT || delim_type == REDIR_HEREDOC)
        return (process_input_redir(*node, str_arr, i));
    else if (delim_type == REDIR_OUTPUT || delim_type == REDIR_APPEND)
        return (process_output_redir(*node, str_arr, i));
    else if (delim_type == REDIR_PIPE)
        return (process_pipe(cmd_list, node, str_arr, i));
    else
    {
        if (!(*node)->cmd)
            return (process_cmd(*node, str_arr, i));
        else
            return (process_cmd_args(*node, str_arr, i));
    }
    return (0);
}

int create_cmd_list(t_cmd_list *cmd_list, char **str_arr)
{
    t_cmd_node *new_node;
    int i;

    if (!str_arr || !cmd_list)
        return (1);
    new_node = (t_cmd_node *){0};
    if (malloc_cmd_node(&new_node))
        return (ft_print_err("Memory allocation error\n"), 1);
    i = 0;
    while (str_arr[i])
    {
        if (process_str_arr(cmd_list, &new_node, str_arr, &i))
            return (free(new_node), 1);
        i++;
    }
    if (new_node->cmd || new_node->args || new_node->input_redir || new_node->output_redir)
        push_cmd_node(cmd_list, new_node);
    else
        free(new_node);
    return (0);
}
