#ifndef EXPANDE_TOKENS_H
#define EXPANDE_TOKENS_H

#include "../../parser.h"

int join_value(char **dest, const char *src, int *errno);
char *get_val_by_key(t_env_list env, const char *name);
int find_key_len(const char *str);
char *get_expanded_val(t_env_list env, char *input, int *i, int *errno);
void update_value(t_token_node *cur, char *new_value, int *errno);

int is_ambig(char *src);
char *squash_spaces(char *src);

#endif