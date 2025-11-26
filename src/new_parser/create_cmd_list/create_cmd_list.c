#include "create_cmd_list_utils.h"

static int set_errno(int code, int *errno)
{
    if (errno)
        *errno = code;
    return (code);
}

static int has_content(t_cmd_node *node)
{
    if (!node)
        return (0);
    return (node->cmd || node->args || node->input_redir || node->output_redir);
}

static int skip_failed_command(t_cmd_node **node, t_token_node **cur,
                               int *errno)
{
    t_token_node *scan;

    free_cmd_node(*node);
    *node = NULL;
    scan = *cur;
    while (scan && scan->type != TOKEN_PIPE)
        scan = scan->next;
    if (!scan)
    {
        *cur = NULL;
        *errno = 0;
        return (0);
    }
    scan = scan->next;
    *cur = scan;
    *errno = 0;
    if (!scan)
        return (0);
    *node = malloc_cmd_node(errno);
    if (*errno)
        return (1);
    (*node)->is_pipe_in = 1;
    return (0);
}

static size_t arg_count(char **args)
{
    size_t count;

    count = 0;
    if (!args)
        return (0);
    while (args[count])
        count++;
    return (count);
}

static int append_arg(t_cmd_node *node, const char *value, int *errno)
{
    char **new_args;
    size_t count;
    size_t i;

    count = arg_count(node->args);
    new_args = ft_calloc(sizeof(char *), count + 2);
    if (!new_args)
        return (set_errno(1, errno));
    i = 0;
    while (i < count)
    {
        new_args[i] = node->args[i];
        i++;
    }
    new_args[count] = ft_strdup(value);
    if (!new_args[count])
        return (free(new_args), set_errno(1, errno));
    free(node->args);
    node->args = new_args;
    return (0);
}

static int handle_word(t_cmd_node *node, t_token_node *tkn, int *errno)
{
    if (!tkn->value)
        return (0);
    if (!node->cmd)
    {
        node->cmd = ft_strdup(tkn->value);
        if (!node->cmd)
            return (*errno = 1);
                append_arg(node, node->cmd, errno);
        if (*errno)
            return (1);
        return (0);
    }
    return (append_arg(node, tkn->value, errno));
}

static int handle_input_redir(t_shell_data *dt, t_cmd_node *node,
                              t_token_node *tkn, int *errno)
{
    if (node->input_redir)
        free(node->input_redir);
    if (tkn->type == TOKEN_HEREDOC || tkn->type == TOKEN_HEREDOC_QUOTES)
    {
        node->input_redir_type = REDIR_HEREDOC;
        if (start_here_doc(dt->env_list, tkn->value, &node->input_redir))
            return (set_errno(1, errno));
    }
    else
    {
        if (check_file_open(tkn->value))
            return (set_errno(1, errno));
        node->input_redir_type = REDIR_INPUT;
        node->input_redir = ft_strdup(tkn->value);
        if (!node->input_redir)
            return (set_errno(1, errno));
    }
    return (0);
}

static int handle_output_redir(t_cmd_node *node, t_token_node *tkn, int *errno)
{
    if (node->output_redir)
        free(node->output_redir);
    node->output_redir_type = (tkn->type == TOKEN_APPEND)
                                  ? REDIR_APPEND
                                  : REDIR_OUTPUT;
    if (check_file_create(tkn->value, node->output_redir_type))
        return (*errno = 1);
    node->output_redir = ft_strdup(tkn->value);
    if (!node->output_redir)
        return (*errno = 1);
    return (0);
}

static int process_token(t_shell_data *dt, t_token_node *tkn,
                         t_cmd_node **node, int *errno)
{
    if (!tkn)
        return (0);
    if (tkn->type == TOKEN_PIPE)
    {
        (*node)->is_pipe_out = 1;
        push_cmd_node(&dt->cmd_list, *node);
        *node = malloc_cmd_node(errno);
        if (*errno)
            return (1);
        (*node)->is_pipe_in = 1;
        return (0);
    }
    if (tkn->type == TOKEN_REDIR_IN || tkn->type == TOKEN_HEREDOC || tkn->type == TOKEN_HEREDOC_QUOTES)
        return (handle_input_redir(dt, *node, tkn, errno));
    if (tkn->type == TOKEN_REDIR_OUT || tkn->type == TOKEN_APPEND)
        return (handle_output_redir(*node, tkn, errno));
    return (handle_word(*node, tkn, errno));
}

static int build_commands(t_shell_data *dt, t_token_node *cur,
                          t_cmd_node **node, t_cmd_node **last, int *errno)
{
    int fatal;

    fatal = 0;
    while (cur && !fatal)
    {
        t_token_node *current_token = cur;
        t_cmd_node *prev_node = *node;

        if (process_token(dt, cur, node, errno) == 0)
        {
            if (current_token->type == TOKEN_PIPE)
                *last = prev_node;
            cur = cur->next;
            continue;
        }
        if (current_token->type == TOKEN_PIPE)
            *last = prev_node;
        fatal = skip_failed_command(node, &cur, errno);
    }
    return (fatal);
}

int create_cmd_list(t_shell_data *dt, t_token_list *tkn_li, int *errno)
{
    t_token_node *cur;
    t_cmd_node *node;
    t_cmd_node *last;
    int fatal;
    if (!dt || !tkn_li || !errno)
        return (1);
    *errno = 0;
    node = malloc_cmd_node(errno);
    if (*errno)
        return (1);
    cur = tkn_li->head;
    if (!cur)
        return (free_cmd_node(node), 0);
    last = NULL;
    fatal = build_commands(dt, cur, &node, &last, errno);
    if (fatal || *errno)
        return (free_cmd_node(node), free_cmd_list(&dt->cmd_list), 0);
    if (has_content(node))
        push_cmd_node(&dt->cmd_list, node), last = node;
    else
        free_cmd_node(node), last ? last->is_pipe_out = 0 : 0;
    return (0);
}
