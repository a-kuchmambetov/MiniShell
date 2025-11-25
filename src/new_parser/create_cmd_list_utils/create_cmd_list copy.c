#include "../main.h"
#include "create_cmd_list_utils/create_cmd_list_utils.h"

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
