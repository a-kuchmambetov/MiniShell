#include "../main.h"
#include "tokenizer_utils/tokenizer_utils.h"

static int	count_tokens(const char *s)
{
    int i;
    int c;

    i = 0;
    c = 0;
    while (s && s[i])
    {
        ft_skip_ws(s, &i);
        if (!s[i])
            break ;
        ft_advance_and_count(s, &i, &c);
    }
    return (c);
}

static char	*make_redir_token(const char *s, int *i)
{
    int consumed;
    int len;
    int olen;
    int j;
    int w;
    char *tok;

    len = ft_len_redir(s, *i, &consumed);
    tok = ft_calloc(len + 1, 1);
    if (!tok)
        return (NULL);
    olen = ft_op_len(s + *i);
    ft_memcpy(tok, s + *i, olen);
    j = *i + olen;
    while (s[j] && ft_is_space(s[j]))
        j++;
    w = ft_word_len(s + j);
    if (w)
    {
        tok[olen] = ' ';
        ft_memcpy(tok + olen + 1, s + j, w);
    }
    *i += consumed;
    return (tok);
}

static char	*make_normal_token(const char *s, int *i)
{
    int consumed;
    int stop;
    int pos;
    int len;
    char *tok;

    len = ft_len_normal(s, *i, &consumed);
    stop = *i + consumed;
    tok = ft_calloc(len + 1, 1);
    if (!tok)
        return (NULL);
    pos = 0;
    ft_fill_normal_token(s, tok, i, stop, &pos);
    tok[pos] = '\0';
    *i = stop;
    return (tok);
}

static char	*next_token(const char *s, int *i)
{
    ft_skip_ws(s, i);
    if (!s[*i])
        return (NULL);
    if (ft_op_len(s + *i) && s[*i] != '|')
        return (make_redir_token(s, i));
    return (make_normal_token(s, i));
}

char	**split_by_delims(const char *s)
{
    int i;
    int k;
    int total;
    char **out;

    total = count_tokens(s);
    out = ft_calloc(total + 1, sizeof(char *));
    if (!out)
        return (NULL);
    i = 0;
    k = 0;
    while (s && s[i])
    {
        out[k] = next_token(s, &i);
        if (!out[k])
            break ;
        k++;
    }
    if (k < total && out[k] == NULL)
    {
        free_str_arr(out);
        return (NULL);
    }
    out[k] = NULL;
    return (out);
}
