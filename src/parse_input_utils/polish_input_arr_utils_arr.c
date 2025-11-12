#include "parse_input_utils.h"

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
    my_free(*dest);
    *dest = new_str;
    return (0);
}

char *join_from_to(char **arr, int from, int to)
{
    char *joined;
    int i;
    size_t total_length;

    if (!arr || from < 0 || to < from)
        return (NULL);
    total_length = 0;
    i = from;
    while (i < to)
        total_length += ft_strlen(arr[i++]);
    joined = ft_calloc(sizeof(char), total_length + 1);
    if (!joined)
        return (NULL);
    i = from;
    while (i < to)
        if (join_value(&joined, arr[i++]))
        {
            my_free(joined);
            return (NULL);
        }
    return (joined);
}

int append_arr(char ***arr, char *input)
{
    char **new_arr;
    int i;

    if (!arr || !*arr || !input)
        return (1);
    new_arr = ft_calloc(sizeof(char *), arr_size(*arr) + 2);
    if (!new_arr)
        return (1);
    i = 0;
    while ((*arr)[i])
    {
        new_arr[i] = ft_strdup((*arr)[i]);
        if (!new_arr[i])
            return (free_str_arr(new_arr), 1);
        i++;
    }
    new_arr[i] = ft_strdup(input);
    if (!new_arr[i])
        return (free_str_arr(new_arr), 1);
    free_str_arr(*arr);
    *arr = new_arr;
    return (0);
}

int append_arr_f(char ***arr, char *input)
{
    int result;

    result = append_arr(arr, input);
    my_free(input);
    return (result);
}

int protect_delim(char **arr)
{
    char *tmp;

    tmp = ft_strdup("\a");
    if (!tmp || join_value(&tmp, arr[0]))
        return (my_free(tmp), 1);
    my_free(arr[0]);
    arr[0] = tmp;
    return (0);
}