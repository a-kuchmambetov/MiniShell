#include "create_token_list.h"


void free_token_list(t_token_list *tkn_li)
{
    t_token_node *current;
    t_token_node *next;

    current = tkn_li->head;
    while (current)
    {
        next = current->next;
        my_free(current->value);
        my_free(current);
        current = next;
    }
    my_free(tkn_li);
}