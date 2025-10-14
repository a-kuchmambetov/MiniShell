#include "../main.h"
// #include "process_expansion_utils/process_expansion_utils.h"
#include "split_input_str_utils/split_input_str_utils.h"

char *get_value_by_key(t_env_list env, const char *name)
{
    t_env_node *current;
    size_t len;

    current = env.first;
    if (!name || !*name)
        return (ft_calloc(1, 1));
    len = ft_strlen(name);
    while (current)
    {
        if (!ft_strncmp(current->key, name, len))
        {
            return (current->value);
        }
        current = current->next;
    }
    return (ft_calloc(1, 1));
}

int find_key_len(const char *str)
{
    int len = 0;

    if (!str || !*str)
        return (0);
    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
    return (len);
}

int join_value(char **dest, const char *src)
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

int get_value_from_str(t_env_list env, t_split_data *dt, const char *input, char **value)
{
    const int key_len = find_key_len(input + dt->i + 1);
    char *key;
    char *env_value;

    if (!input)
        return (1);
    if (key_len < 1)
    {
        *value = ft_calloc(1, 1);
        if (!*value)
            return (1);
        return (0);
    }
    key = ft_strndup(input + dt->i + 1, key_len);
    if (!key)
        return (1);
    env_value = get_value_by_key(env, key);
    if (env_value)
        *value = ft_strdup(env_value);
    else
        *value = ft_calloc(1, 1);
    if (!*value)
        return (1);
    dt->i += key_len;
    return (0);
}

int add_value_to_arr(t_split_data *dt, t_env_list env, char **input)
{
    char **value;

    if (!dt || !dt->arr)
        return (1);
    value = malloc(sizeof(char *) * 1);
    if (!value)
        return (free_str_arr(dt->arr), 1);
    if (get_value_from_str(env, dt, *input, value))
        return (ft_print_err("Error: get value failed\n"),
                free_str_arr(dt->arr), 1);
    if (add_row(dt, *value))
        return (ft_print_err("Error: split input failed\n"),
                free_str_arr(dt->arr), 1);
    free(value);
    return (0);
}

int join_arr(t_split_data *dt, char **input)
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
    if (!input || !*input)
        return (1);
    while (s[dt.i])
    {
        if (s[dt.i] == '$')
        {
            if (add_row(&dt, ft_strndup(s + dt.l, dt.i - dt.l)))
                return (ft_print_err("Error: split input failed\n"),
                        free_str_arr(dt.arr), 1);
            if (add_value_to_arr(&dt, env, input))
                return (1);
            dt.l = dt.i + 1;
        }
        dt.i++;
    }
    free(*input);
    if (join_arr(&dt, input))
        return (1);
    return (0);
}

int check_do_expansion(t_env_list env, char **input)
{
    char *value;

    value = NULL;
    if (!input || !*input)
        return (0);
    if ((*input)[0] == '"')
    {
        value = ft_strtrim(*input, "\"");
        free(*input);
        *input = value;
    }
    else if ((*input)[0] == '\'')
    {
        value = ft_strtrim(*input, "\'");
        free(*input);
        *input = value;
        return (0);
    }
    process_expansion(env, input);
    return (0);
}
