#include "tokenizer_utils.h"

int ft_handle_pipe_len(const char *s, int *i, int *len)
{
    if (s[*i] != '|')
        return (0);
    if (*len)
        *len += 2;
    else
        *len += 1;
    (*i)++;
    while (s[*i] && ft_is_space(s[*i]))
        (*i)++;
    return (1);
}

int ft_add_word_len(const char *s, int *i, int *len)
{
    int w;

    w = ft_word_len(s + *i);
    if (w <= 0)
        return (0);
    if (*len)
        *len += 1;
    *len += w;
    *i += w;
    return (1);
}

int ft_len_normal(const char *s, int i, int *consumed)
{
    int start;
    int len;

    start = i;
    len = 0;
    while (s[i])
    {
        while (s[i] && ft_is_space(s[i]))
            i++;
        if (!s[i])
            break ;
        if (ft_handle_pipe_len(s, &i, &len))
            break ;
        if (ft_op_len(s + i) && s[i] != '|')
            break ;
        if (!ft_add_word_len(s, &i, &len))
            break ;
    }
    *consumed = i - start;
    return (len);
}

int ft_len_redir(const char *s, int i, int *consumed)
{
    int olen;
    int j;
    int w;

    olen = ft_op_len(s + i);
    j = i + olen;
    while (s[j] && ft_is_space(s[j]))
        j++;
    w = ft_word_len(s + j);
    *consumed = (j + w) - i;
    return (olen + (w ? 1 + w : 0));
}

void ft_advance_and_count(const char *s, int *i, int *c)
{
    int tmp;

    if (ft_op_len(s + *i) && s[*i] != '|')
        ft_len_redir(s, *i, &tmp);
    else
        ft_len_normal(s, *i, &tmp);
    *i += tmp;
    (*c)++;
}

