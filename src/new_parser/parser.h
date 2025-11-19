#ifndef PARSER_H
#define PARSER_H

#include "../../main.h"
#include "split_input/split_input.h"
#include "token/create_token_list/create_token_list.h"

typedef enum e_token_type
{
    HEREDOC = 0,
    REDIR_IN = 1,
    REDIR_OUT = 2,
    APPEND = 3,
    PIPE = 4,
    TEXT = 5,
    EXPANSION = 6
} t_token_type;

typedef struct s_token_node
{
    char *value;
    t_token_type type;
    int is_space_after;
    struct s_token_node *next;
} t_token_node;

typedef struct s_token_list
{
    t_token_node *head;
    t_token_node *tail;
} t_token_list;

t_token_list *create_token_list(char **arr, int *errno);

#endif