#include "tokenizer_utils.h"

int ft_word_len(const char *s)
{
    int i;

    i = 0;
    while (s[i] && !ft_is_space(s[i]) && !ft_op_len(s + i))
        i++;
    return (i);
}

void ft_skip_ws(const char *s, int *i)
{
    while (s[*i] && ft_is_space(s[*i]))
        (*i)++;
}

