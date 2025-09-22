#include "../main.h"
// THAT IS HELL
// Correct input:
// cat<main.c>test - read from redirection file and write to output file
// cat Makefile<main.c>test main.h - read from files (Makefile and main.h) (redirection ignored cause files names given) and write to output file
// <Makefile cat main.c main.h|cat>test - read from files (main.c, main.h) (redirection ignored cause files names given) and redirect out to pipe and read from pipe and write to output file
// <Makefile cat main.c main.h|>test - read from files (main.c, main.h) (redirection ignored cause files names given) and redirect out to pipe and read from pipe but ignored cause no cmd and write to output filem but no input so nothing happens (just create empty file)

static int is_delimiter(const char *str)
{
    if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
        return (2);
    if (str[0] == '|' || str[0] == '>' || str[0] == '<' || str[0] == ' ')
        return (1);
    return (0);
}

static int add_row(t_split_data *dt, char *s)
{
    char **new_arr;
    int i;

    if (!s)
        return (1);
    new_arr = ft_calloc(sizeof(char *), dt->row + 2);
    if (!new_arr)
        return (free(s),1);
    i = 0;
    while (i < dt->row)
    {
        new_arr[i] = dt->arr[i];
        i++;
    }
    new_arr[i] = s;
    free(dt->arr);
    dt->arr = new_arr;
    dt->row++;
    return (0);
}

static void skip_spaces(const char *s, t_split_data *dt)
{
    if (s[dt->i] == ' ' && dt->i - dt->l < 1)
        dt->l = dt->i + 1;
}

char **split_input_str(const char *s)
{
    t_split_data dt;
    int delim_len;

    dt = (t_split_data){0};
    while (s[dt.i])
    {
        skip_spaces(s, &dt);
        delim_len = is_delimiter(s + dt.i);
        if (delim_len > 0)
        {
            if (dt.i - dt.l > 0)
                if (add_row(&dt, ft_strndup(s + dt.l, dt.i - dt.l)))
                    return (ft_print_err("Error: split input failed\n"), free_str_arr(dt.arr), NULL);
            if (s[dt.i] != ' ')
                if (add_row(&dt, ft_strndup(s + dt.i, delim_len)))
                    return (ft_print_err("Error: split input failed\n"), free_str_arr(dt.arr), NULL);
            dt.l = dt.i + delim_len;
            dt.i += delim_len;
        }
        else
            dt.i++;
    }
    if (dt.i - dt.l > 0)
        if (add_row(&dt, ft_strndup(s + dt.l, dt.i - dt.l)))
            return (ft_print_err("Error: split input failed\n"), free_str_arr(dt.arr), NULL);
    return (dt.arr);
}
