#include "executor.h"

typedef struct s_arg_state
{
    const char  *src;
    size_t      i;
    size_t      start;
    size_t      count;
    char        **arr;
}   t_arg_state;

static void init_state(t_arg_state *st, const char *args)
{
    st->src = args;
    st->i = 0;
    st->start = 0;
    st->count = 0;
    st->arr = NULL;
}

static void skip_spaces(t_arg_state *st)
{
    while (st->src[st->i] == ' ' || st->src[st->i] == '\t')
        st->i++;
}

static void consume_token(t_arg_state *st)
{
    char quote;

    while (st->src[st->i] && st->src[st->i] != ' ' && st->src[st->i] != '\t')
    {
        if (st->src[st->i] == '\'' || st->src[st->i] == '"')
        {
            quote = st->src[st->i++];
            while (st->src[st->i] && st->src[st->i] != quote)
                st->i++;
            if (st->src[st->i] == quote)
                st->i++;
        }
        else
            st->i++;
    }
}

static int push_token(t_arg_state *st)
{
    char    **new_arr;
    size_t  idx;
    char    *token;

    token = ft_strndup(st->src + st->start, st->i - st->start);
    if (!token)
        return (1);
    new_arr = ft_calloc(sizeof(char *), st->count + 2);
    if (!new_arr)
        return (free(token), 1);
    idx = 0;
    while (idx < st->count)
    {
        new_arr[idx] = st->arr[idx];
        idx++;
    }
    new_arr[idx] = token;
    free(st->arr);
    st->arr = new_arr;
    st->count++;
    return (0);
}

char **split_args(const char *args, int *errored)
{
    t_arg_state st;

    if (errored)
        *errored = 0;
    if (!args || !args[0])
        return (NULL);
    init_state(&st, args);
    while (st.src[st.i])
    {
        skip_spaces(&st);
        if (!st.src[st.i])
            break;
        st.start = st.i;
        consume_token(&st);
        if (push_token(&st))
        {
            free_str_arr(st.arr);
            if (errored)
                *errored = 1;
            return (NULL);
        }
    }
    return (st.arr);
}
