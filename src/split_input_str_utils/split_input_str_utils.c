#include "split_input_str_utils.h"

int is_delimiter(const char *str)
{
    if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
        return (2);
    if (str[0] == '|' || str[0] == '>' || str[0] == '<' || str[0] == ' ')
        return (1);
    return (0);
}

void skip_spaces(const char *s, t_split_data *dt)
{
    if (s[dt->i] == ' ' && dt->i - dt->l < 1)
        dt->l = dt->i + 1;
    dt->i++;
}

int add_row(t_split_data *dt, char *s)
{
    char **new_arr;
    int i;

    if (!s)
        return (1);
    new_arr = ft_calloc(sizeof(char *), dt->row + 2);
    if (!new_arr)
        return (free(s), 1);
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

int handle_add_row(const char *s, t_split_data *dt)
{
    if (add_row(dt, ft_strndup(s + dt->l, dt->i - dt->l)))
        return (ft_print_err("Error: split input failed\n"),
                free_str_arr(dt->arr), 1);
    return (0);
}

int process_delimiter(const char *s, t_split_data *dt, int delim_len)
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