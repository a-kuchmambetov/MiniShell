#include "../main.h"
#include "process_expansion_utils/process_expansion_utils.h"
#include "split_input_str_utils/split_input_str_utils.h"

static int join_value(char **dest, const char *src)
{
    char *new_str;

    if (!dest || !src)
        return (1);
    if (!*dest)
        *dest = ft_calloc(1, 1);
    new_str = ft_strjoin(*dest, src);
    if (!new_str)
        return (1);
    free(*dest);
    *dest = new_str;
    return (0);
}

static int add_value_to_arr(t_split_data *dt, t_env_list env, char **input)
{
    char **value;

    if (!dt || !dt->arr)
        return (1);
    value = malloc(sizeof(char *) * 1);
    if (!value)
        return (free_str_arr(dt->arr), 1);
    if (get_value_from_str(env, dt, *input, value))
        return (ft_print_err("error: get value failed\n"),
                free(value), free_str_arr(dt->arr), 1);
    if (add_row(dt, *value))
        return (ft_print_err("error: expanding input failed\n"),
                free(value), free_str_arr(dt->arr), 1);
    free(value);
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

int process_expansion(t_env_list env, char **input)
{
    const char *s = *input;
    t_split_data dt;

    dt = (t_split_data){0};
    while (s[dt.i])
    {
        if (s[dt.i] == '$')
        {
            if (add_row(&dt, ft_strndup(s + dt.l, dt.i - dt.l)))
                return (ft_print_err("error: expanding input failed\n"),
                        free_str_arr(dt.arr), 1);
            if (add_value_to_arr(&dt, env, input))
                return (1);
            dt.l = dt.i + 1;
        }
        dt.i++;
    }
    if (dt.i > dt.l)
        if (add_row(&dt, ft_strndup(s + dt.l, dt.i - dt.l)))
            return (ft_print_err("error: expanding input failed\n"),
                    free_str_arr(dt.arr), 1);
    free(*input);
    *input = NULL;
    if (join_arr(&dt, input))
        return (1);
    return (0);
}

// int check_do_expansion(t_env_list env, char **input)
// {
//     char *value;

//     value = NULL;
//     if (!input || !*input)
//         return (0);
//     if ((*input)[0] == '"')
//     {
//         value = ft_strtrim(*input, "\"");
//         free(*input);
//         *input = value;
//     }
//     else if ((*input)[0] == '\'')
//     {
//         value = ft_strtrim(*input, "\'");
//         free(*input);
//         *input = value;
//         return (0);
//     }
//     if (!input || !*input)
//         return (1);
//     if (process_expansion(env, input))
//         return (1);
//     return (0);
// }

int check_do_expansion(t_env_list env, char **input)
{
    char *value;

    if (!input || !*input)
        return 0;

    // Якщо токен у подвійних лапках — просто знімаємо ЗОВНІШНІ лапки і ВИХОДИМО.
    // НІЯКОГО process_expansion тут, щоб не чіпати пробіли всередині!
    if ((*input)[0] == '"')
    {
        size_t n = ft_strlen(*input);
        if (n >= 2 && (*input)[n - 1] == '"')
        {
            value = ft_substr(*input, 1, n - 2);
            if (!value) return 1;
            free(*input);
            *input = value;
        }
        return 0;
    }

    // Якщо одинарні лапки — теж тільки зняти зовнішні і вийти
    if ((*input)[0] == '\'')
    {
        size_t n = ft_strlen(*input);
        if (n >= 2 && (*input)[n - 1] == '\'')
        {
            value = ft_substr(*input, 1, n - 2);
            if (!value) return 1;
            free(*input);
            *input = value;
        }
        return 0;
    }

    // Якщо немає змінних — нічого не робимо
    if (!ft_strchr(*input, '$'))
        return 0;

    // Інакше робимо expansion
    if (process_expansion(env, input))
        return 1;

    return 0;
}
