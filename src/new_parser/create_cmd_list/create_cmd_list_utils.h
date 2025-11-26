#ifndef CREATE_CMD_LIST_UTILS_H
#define CREATE_CMD_LIST_UTILS_H

#include "../parser.h"

void push_cmd_node(t_cmd_list *cmd_list, t_cmd_node *new_node);
t_cmd_node *malloc_cmd_node(int *errno);

int check_file_open(const char *filename);
int check_file_create(const char *filename, const int redir_type);

#endif
