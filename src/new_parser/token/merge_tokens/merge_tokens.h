#ifndef MERGE_TOKENS_H
#define MERGE_TOKENS_H

#include "../../parser.h"

void merge_tokens(t_token_list tkn_li, int *errno);

char *trim_quotes(char *str);
char *trim_space_before(char *str);
char *trim_space_after(char *str);
char *trim_space_both(char *str);

#endif