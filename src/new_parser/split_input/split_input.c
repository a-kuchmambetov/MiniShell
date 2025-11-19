#include "split_input.h"
// THAT IS HELL
// Correct input:
// cat<main.c>test - read from redirection file and write to output file
// cat Makefile<main.c>test main.h - read from files (Makefile and main.h) (redirection ignored cause files names given) and write to output file
// <Makefile cat main.c main.h|cat>test - read from files (main.c, main.h) (redirection ignored cause files names given) and redirect out to pipe and read from pipe and write to output file
// <Makefile cat main.c main.h|>test - read from files (main.c, main.h) (redirection ignored cause files names given) and redirect out to pipe and read from pipe but ignored cause no cmd and write to output filem but no input so nothing happens (just create empty file)

static int quote_parser(const char *s, t_split_data *dt, const char quote_char)
{
    dt->in_quote = YES_QUOTE;
    if (dt->i - dt->l > 0)
    {
        if (handle_add_row(s, dt))
            return (1);
        dt->l = dt->i;
    }
    dt->i++;
    while (s[dt->i] && dt->in_quote != NO_QUOTE)
    {
        if (s[dt->i] == quote_char)
            dt->in_quote = NO_QUOTE;
        dt->i++;
    }
    if (s[dt->i] == '\0' && dt->in_quote != NO_QUOTE)
        return (ft_print_err("Error: Unclosed quote\n"),
                free_str_arr(dt->arr), 1);
    if (handle_add_row(s, dt))
        return (1);
    dt->l = dt->i;
    return (0);
}

static int regular_parser(const char *s, t_split_data *dt)
{
    int delim_len;

    delim_len = 0;
    if (dt->i > 0 && s[dt->i - 1] == ' ' && s[dt->i] == ' ')
    {
        skip_spaces(s, dt);
        return (0);
    }
    delim_len = is_delim(s + dt->i);
    if (delim_len > 0)
    {
        if (process_delimiter(s, dt, delim_len))
            return (1);
    }
    else
        dt->i++;
    return (0);
}

static void select_parser(const char *s, t_split_data *dt, int *errno)
{
    const int str_len = ft_strlen(s);

    if ((s[dt->i] == '\'' || s[dt->i] == '\"') && s[dt->i] == s[str_len - 1])
    {
        *errno = quote_parser(s, dt, s[dt->i]);
        if (*errno)
            return (NULL);
    }
    else
    {
        *errno = regular_parser(s, dt);
        if (*errno)
            return (NULL);
    }
    *errno = 0;
}

char **split_input(const char *s, int parse_quotes, int *errno)
{
    t_split_data dt;
    int delim_len;

    delim_len = 0;
    dt = (t_split_data){0};
    while (s[dt.i])
    {
        select_parser(s, &dt, errno);
        if (*errno)
            return (NULL);
    }
    if (dt.i - dt.l > 0)
    {
        *errno = handle_add_row(s, &dt);
        if (*errno)
            return (NULL);
    }
    *errno = 0;
    return (dt.arr);
}
