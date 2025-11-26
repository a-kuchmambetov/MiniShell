#include "create_token_list.h"

void free_token_node(t_token_node *node)
{
    my_free(node->value);
    my_free(node);
}