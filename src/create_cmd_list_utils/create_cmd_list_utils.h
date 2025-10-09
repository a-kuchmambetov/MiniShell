#ifndef CREATE_CMD_LIST_UTILS_H
#define CREATE_CMD_LIST_UTILS_H

#include "../../main.h"

void push_cmd_node(t_cmd_list *cmd_list, t_cmd_node *new_node);
int malloc_cmd_node(t_cmd_node **new_node);

int check_file_open(const char *filename);
int check_file_create(const char *filename, const int redir_type);

int set_input_redir(t_cmd_node *node, char **str_arr, int *i);
int set_output_redir(t_cmd_node *node, char **str_arr, int *i);
int set_pipe(t_cmd_list *cmd_list, t_cmd_node **node,
             char **str_arr, int *i);
int set_cmd(t_cmd_node *node, char **str_arr, int *i);
int set_cmd_args(t_cmd_node *node, char **str_arr, int *i);

#endif