#include "tokenizer_utils.h"

int ft_append_pipe(const char *s, char *dst, int *i, int *pos)
{
    if (s[*i] != '|')
        return (0);
    if (*pos)
        dst[(*pos)++] = ' ';
    dst[(*pos)++] = '|';
    (*i)++;
    while (s[*i] && ft_is_space(s[*i]))
        (*i)++;
    return (1);
}

int ft_copy_word(const char *s, char *dst, int *i, int *pos)
{
    int w;

    w = ft_word_len(s + *i);
    if (w <= 0)
        return (0);
    if (*pos)
        dst[(*pos)++] = ' ';
    ft_memcpy(dst + *pos, s + *i, w);
    *pos += w;
    *i += w;
    return (1);
}

void ft_fill_normal_token(
    const char *s, char *tok, int *i, int stop, int *pos)
{
    while (*i < stop)
    {
        ft_skip_ws(s, i);
        if (ft_append_pipe(s, tok, i, pos))
            break ;
        if (ft_op_len(s + *i) && s[*i] != '|')
            break ;
        if (!ft_copy_word(s, tok, i, pos))
            break ;
    }
}

