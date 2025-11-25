#ifndef MERGE_TOKENS_H
#define MERGE_TOKENS_H

#include "new_parser/parser.h"

void merge_token(t_token_list tkn_li, int *errno);

char *trim_quotes(const char *str);
char *trim_space_before(const char *str);
char *trim_space_after(const char *str);

#endif