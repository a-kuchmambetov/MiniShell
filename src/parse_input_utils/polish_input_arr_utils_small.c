#include "parse_input_utils.h"

int is_delim(const char *str)
{
    const int str_len = ft_strlen(str);

    if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
        return (2);
    if ((str[0] == '|' || str[0] == '>' || str[0] == '<' || str[0] == ' ') && str_len == 1)
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

int arr_size(char **arr)
{
    int size;

    size = 0;
    if (!arr || !*arr)
        return (0);
    while (arr[size])
        size++;
    return (size);
}

int dup_arr(char ***dest, char **src)
{
    int i;

    i = 0;
    while (src[i])
    {
        if (append_arr(dest, src[i]))
            return (1);
        i++;
    }
    return (0);
}

int find_space_index(char **src)
{
    const int src_size = arr_size(src);
    int i;

    i = 0;
    while (i < src_size)
    {
        if (src[i][0] == ' ' && src[i][1] == '\0')
            return (i);
        i++;
    }
    return (src_size);
}