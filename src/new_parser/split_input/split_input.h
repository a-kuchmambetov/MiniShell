#ifndef SPLIT_INPUT_STR_H
#define SPLIT_INPUT_STR_H

#include "../parser.h"

char **split_input(const char *s, int parse_quotes, int *errno);

int is_delim(const char *str);
void skip_spaces(const char *s, t_split_data *dt);
int handle_add_row(const char *s, t_split_data *dt);
int add_row(t_split_data *dt, char *s);
int process_delimiter(const char *s, t_split_data *dt, int delim_len);

#endif