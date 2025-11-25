#include "../main.h"
#include "create_cmd_list_utils/create_cmd_list_utils.h"

static t_token_node *skip_node(t_token_node *cur, t_cmd_node **new_node, int *errno)
{
    free_cmd_node(new_node);
    *new_node = malloc_cmd_node(errno);
    if (*errno)
        return (NULL);
    while (cur && cur->type != TOKEN_PIPE)
        cur = cur->next;
    return (cur);
}

static int process_token(t_shell_data *dt, t_cmd_node **new_node,
                         t_token_node *cur, int *errno)
{
    if (!new_node)
    {
        *new_node = malloc_cmd_node(errno);
        if (*errno)
            return (1);
    }
    if (cur && cur->type == TOKEN_PIPE)
    {
        (*new_node)->is_pipe_out = 1;
        add_cmd_node_to_list(dt, &new_node, errno);
        if (*errno)
            return (1);
        *new_node = malloc_cmd_node(errno);
        if (*errno)
            return (1);
        (*new_node)->is_pipe_in = 1;
        return (0);
    }
    if (fill_node(new_node, cur, errno))
        return (1);
    return (0);
}

int create_cmd_list(t_shell_data *dt, t_token_list *tkn_li, int *errno)
{
    t_token_node *cur;
    t_cmd_node *new_node;

    if (!dt || !tkn_li)
        return (1);
    cur = tkn_li->head;

    while (cur)
    {
        if (process_token(dt, &new_node, cur, errno))
        {
            cur = skip_node(cur, &new_node, errno);
            continue;
        }
        if (*errno)
            return (1);
        cur = cur->next;
    }
    return (0);
}
