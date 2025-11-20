#include "parser.h"

void expand_tokens(t_token_list *token_list, t_shell_data *data, int *errno)
{
    t_token_node *current;

    if (!token_list || !data)
    {
        *errno = 1;
        return;
    }
    current = token_list->head;
    while (current)
    {
        if (check_do_expansion(data->env_list, &current->value))
        {
            *errno = 1;
            return ;
        }
        current = current->next;
    }
    return;
}
