#ifndef SPLIT_INPUT_STR_H
#define SPLIT_INPUT_STR_H

#include "../../main.h"

int is_delimiter(const char *str);
void skip_spaces(const char *s, t_split_data *dt);
int handle_add_row(const char *s, t_split_data *dt);
int add_row(t_split_data *dt, char *s);

#endif