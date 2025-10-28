#include "../main.h"

int is_delim(const char *str)
{
    if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
        return (2);
    if (str[0] == '|' || str[0] == '>' || str[0] == '<' || str[0] == ' ')
        return (1);
    return (0);
}

int is_quoted(const char *str)
{
    const int str_len = ft_strlen(str);

    if (str_len < 2)
        return (0);
    if ((str[0] == '\'' || str[0] == '"') && str[str_len - 1] == str[0])
        return (1);
    return (0);
}

int array_size(char **arr)
{
    int size;

    size = 0;
    if (!arr)
        return (0);
    while (arr[size])
        size++;
    return (size);
}

int split_add_to_arr(char ***arr, const char *str, int index)
{
    char **new_array;

    if (!arr || !str)
        return (1);
    new_array = ft_calloc(sizeof(char *), array_size(*arr) + 2);
    
};

int split_unquoted_space(char ***arr)
{
    char **new_arr;
    int i;
    int status_code;

    if (!arr || !*arr)
        return (1);
    new_arr = NULL;
    i = 0;
    while ((*arr)[i])
    {
        if (!is_quoted((*arr)[i]) && ft_strchr((*arr)[i], ' '))
        {
            status_code = split_and_insert_at(&new_arr, (*arr)[i], i);
            i++;
        }
        else
            status_code = rm_quotes_add_to_arr(&new_arr, (*arr)[i]);
        if (status_code != 0)
            return (status_code);
        i++;
    }
    free_str_arr(*arr);
    *arr = new_arr;
    return (0);
}

int polish_input_arr(char **arr)
{
    char **new_arr;
    int i;

    if (!arr || !*arr)
        return (1);
    i = 0;
    while (arr[i])
    {
        if (is_delim(arr[i]) && (is_delim(arr[i + 1]) || arr[i + 1]))
        {
            i++;
            continue;
        }
        i++;
    }
    return (0);
}
