#include "../main.h"
#include "process_expansion_utils/process_expansion_utils.h"
#include "split_input_str_utils/split_input_str_utils.h"

static int join_value(char **dest, const char *src)
{
    char *new_str;

    if (!dest || !src)
        return (1);
    if (!*dest)
    {
        *dest = ft_calloc(1, 1);
        if (!*dest)
            return (1);
    }
    new_str = ft_strjoin(*dest, src);
    if (!new_str)
        return (my_free(*dest), 1);
    my_free(*dest);
    *dest = new_str;
    return (0);
}

static int add_value_to_arr(t_split_data *dt, t_env_list env, char **input)
{
    char *value;

    if (!dt || !dt->arr)
        return (1);
    value = NULL;
    if (get_value_from_str(env, dt, *input, &value))
        return (ft_print_err("error: get value failed\n"),
                free_str_arr(dt->arr), 1);
    if (add_row(dt, value))
        return (ft_print_err("error: expanding input failed\n"),
                my_free(value), free_str_arr(dt->arr), 1);
    return (0);
}

static int join_arr(t_split_data *dt, char **input)
{
    char *new_str;
    int i;

    if (!dt || !dt->arr || !input)
        return (1);
    new_str = ft_calloc(1, 1);
    if (!new_str)
        return (free_str_arr(dt->arr), 1);
    i = 0;
    while (i < dt->row)
    {
        if (join_value(&new_str, dt->arr[i]))
            return (free(new_str), free_str_arr(dt->arr), 1);
        i++;
    }
    free_str_arr(dt->arr);
    *input = new_str;
    return (0);
}

int process_expansion(t_env_list env, char **inp)
{
    t_split_data dt;
    char *temp;

    dt = (t_split_data){0};
    while ((*inp)[dt.i])
    {
        if ((*inp)[dt.i] == '$')
        {
            temp = ft_strndup(*inp + dt.l, dt.i - dt.l);
            if (!temp || add_row(&dt, temp))
                return (my_free(temp), free_str_arr(dt.arr), 1);
            if (add_value_to_arr(&dt, env, inp))
                return (1);
            dt.l = dt.i + 1;
        }
        dt.i++;
    }
    if (dt.i > dt.l)
    {
        temp = ft_strndup(*inp + dt.l, dt.i - dt.l);
        if (!temp || add_row(&dt, temp))
            return (my_free(temp), free_str_arr(dt.arr), 1);
    }
    my_free(*inp);
    return (join_arr(&dt, inp));
}

int check_do_expansion(t_env_list env, char **input)
{
    if (!input || !*input)
        return (1);
    if ((*input)[0] == '\'' || ft_strncmp(*input, " ", 1) == 0)
        return (0);
    if (process_expansion(env, input))
        return (1);
    return (0);
}
