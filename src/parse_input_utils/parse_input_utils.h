#ifndef PARSE_INPUT_UTILS_H
#define PARSE_INPUT_UTILS_H

#include "../../main.h"

int expand_input_arr(t_shell_data *data, char **arr);

// polish_input_arr_utils_small.c
int is_delim(const char *str);
int is_quoted(const char *str);
int arr_size(char **arr);
int dup_arr(char ***dest, char **src);
int find_space_index(char **src);

// polish_input_arr_utils_arr.c
int join_value(char **dest, const char *src);
char *join_from_to(char **arr, int from, int to);
int append_arr(char ***arr, char *input);
int append_arr_f(char ***arr, char *input);
int protect_delim(char **arr);

#endif