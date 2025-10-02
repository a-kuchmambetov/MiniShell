#include "tokenizer_utils.h"

int ft_is_space(char c)
{
    if (c == ' ' || c == '\t')
        return (1);
    return (0);
}

int ft_op_len(const char *s)
{
    if (!s || !*s)
        return (0);
    if (s[0] == '<' && s[1] == '<')
        return (2);
    if (s[0] == '>' && s[1] == '>')
        return (2);
    if (s[0] == '<' || s[0] == '>' || s[0] == '|')
        return (1);
    return (0);
}

