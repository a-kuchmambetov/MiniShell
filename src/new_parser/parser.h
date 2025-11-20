#ifndef PARSER_H
#define PARSER_H

#include "../../main.h"

typedef struct s_token_node t_token_node;
typedef struct s_token_list t_token_list;

typedef enum e_token_type
{
    TOKEN_HEREDOC = 0,
    TOKEN_REDIR_IN = 1,
    TOKEN_REDIR_OUT = 2,
    TOKEN_APPEND = 3,
    TOKEN_PIPE = 4,
    TOKEN_TEXT = 5,
    TOKEN_EXPANSION = 6
} t_token_type;

struct s_token_node
{
    char *value;
    t_token_type type;
    int is_space_after;
    struct s_token_node *next;
};

struct s_token_list
{
    t_token_node *head;
    t_token_node *tail;
};

char **split_input(const char *s, int *errno);
t_token_list *create_token_list(char **arr, int *errno);
void expand_tokens(t_token_list *token_list, t_shell_data *data, int *errno);

#endif