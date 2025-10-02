#include "../main.h"
#include "split_input_str_utils/split_input_str_utils.h"
// THAT IS HELL
// Correct input:
// cat<main.c>test - read from redirection file and write to output file
// cat Makefile<main.c>test main.h - read from files (Makefile and main.h) (redirection ignored cause files names given) and write to output file
// <Makefile cat main.c main.h|cat>test - read from files (main.c, main.h) (redirection ignored cause files names given) and redirect out to pipe and read from pipe and write to output file
// <Makefile cat main.c main.h|>test - read from files (main.c, main.h) (redirection ignored cause files names given) and redirect out to pipe and read from pipe but ignored cause no cmd and write to output filem but no input so nothing happens (just create empty file)

static int process_delimiter(const char *s, t_split_data *dt, int delim_len)
{
    if (dt->i - dt->l > 0)
        if (add_row(dt, ft_strndup(s + dt->l, dt->i - dt->l)))
            return (ft_print_err("Error: split input failed\n"),
                    free_str_arr(dt->arr), 1);
    if (add_row(dt, ft_strndup(s + dt->i, delim_len)))
        return (ft_print_err("Error: split input failed\n"),
                free_str_arr(dt->arr), 1);
    dt->l = dt->i + delim_len;
    dt->i += delim_len;
    return (0);
}

static int quote_parse(const char *s, t_split_data *dt, const char quote_char)
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

static int regular_parser(const char *s, t_split_data *dt, int delim_len)
{
    if (dt->i > 0 && s[dt->i - 1] == ' ' && s[dt->i] == ' ')
    {
        skip_spaces(s, dt);
        return (0);
    }
    delim_len = is_delimiter(s + dt->i);
    if (delim_len > 0)
    {
        if (process_delimiter(s, dt, delim_len))
            return (1);
    }
    else
        dt->i++;
    return (0);
}

char **split_input_str(const char *s)
{
    t_split_data dt;
    int delim_len;

    delim_len = 0;
    dt = (t_split_data){0};
    while (s[dt.i])
    {
        if (s[dt.i] == '\'' || s[dt.i] == '\"')
        {
            if (quote_parse(s, &dt, s[dt.i]))
                return (NULL);
        }
        else if (regular_parser(s, &dt, delim_len))
            return (NULL);
    }
    if (dt.i - dt.l > 0)
        if (handle_add_row(s, &dt))
            return (NULL);
    return (dt.arr);
}
