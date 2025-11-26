#ifndef CREATE_TOKEN_LIST_UTILS_H
#define CREATE_TOKEN_LIST_UTILS_H

#include "../../parser.h"

void free_token_list(t_token_list *tkn_li);
void free_token_node(t_token_node *node);

t_token_list *create_token_list(char **arr, int *errno);
t_token_node *set_tkn(char **arr, int *i, int *errno);

#endif