#ifndef START_HERE_DOC_UTILS_H
#define START_HERE_DOC_UTILS_H

#include "../../main.h"

int check_signal_hook(void);
void handle_sigint_heredoc(int sig);

int check_file_name(char **name);
int create_file(char **filename, int *fd);

void print_error(const char *eof_word);
int compare_eof(const char *input, const char *eof_word);

#endif