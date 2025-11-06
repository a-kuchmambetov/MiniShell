#include "../main.h"

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

static int append_arr(char ***arr, char *input)
{
    char **new_arr;
    int i;

    if (!arr || !*arr || !input || !*input)
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
    // my_free(input);
    *arr = new_arr;
    return (0);
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

int protect_delim(char **arr)
{
    char *tmp;

    tmp = ft_strdup("\a");
    if (join_value(&tmp, arr[0]))
        return (1);
    my_free(arr[0]);
    arr[0] = tmp;
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

int merge_splited_space(char ***dst_a, char **src_arr)
{
    const int src_size = arr_size(src_arr);
    t_polish_data dt;

    dt = (t_polish_data){0};
    if (!dst_a || !*dst_a || !src_arr)
        return (1);
    if (is_delim(src_arr[0]) && protect_delim(src_arr))
        return (1);
    dt.to = find_space_index(src_arr);
    if (append_arr(dst_a, join_from_to(src_arr, dt.from, dt.to)))
        return (1);
    dt.from = dt.to;
    if (dt.from + 1 < src_size)
        if (append_arr(dst_a, ft_strdup(" ")) ||
            append_arr(dst_a, join_from_to(src_arr, dt.from + 1, src_size)))
            return (1);
    free_str_arr(src_arr);
    return (0);
}

static int final_merge_util(char ***dst_a, char **src_arr, t_polish_data *dt)
{
    int code;

    code = 0;
    dt->to = dt->i;
    if (dt->from == dt->to)
        code = append_arr(dst_a, src_arr[dt->i]);
    else
    {
        code = append_arr(dst_a, join_from_to(src_arr, dt->from, dt->to));
        if (code)
            return (1);
        code = append_arr(dst_a, src_arr[dt->i]);
    }
    dt->from = dt->i + 1;
    if (code)
        return (1);
    return (0);
}

int final_merge(char ***dst_a, char **src_arr)
{
    const int src_size = arr_size(src_arr);
    t_polish_data dt;

    dt = (t_polish_data){0};
    dt.arr = ft_calloc(sizeof(char *), 1);
    if (dt.arr == NULL)
        return (1);
    while (dt.i < src_size)
    {
        if (is_delim(src_arr[dt.i]))
        {
            if (final_merge_util(&dt.arr, src_arr, &dt))
                return (free_str_arr(dt.arr), 1);
        }
        dt.i++;
    }
    if (dt.from < src_size)
    {
        if (append_arr(&dt.arr, join_from_to(src_arr, dt.from, src_size)))
            return (free_str_arr(dt.arr), 1);
    }
    free_str_arr(*dst_a);
    free_str_arr(src_arr);
    *dst_a = dt.arr;
    return (0);
}

char **split_space(t_polish_data dt)
{
    char **new_arr;
    char *trimmed_str;
    int code;

    new_arr = ft_calloc(sizeof(char *), 1);
    while (dt.arr[dt.i])
    {
        if (is_delim(dt.arr[dt.i]) && !is_quoted(dt.arr[dt.i]))
            code = append_arr(&new_arr, dt.arr[dt.i]);
        else if (is_quoted(dt.arr[dt.i]))
            code = append_arr(&new_arr, ft_strtrim(dt.arr[dt.i], "'\""));
        else
        {
            trimmed_str = ft_strtrim(dt.arr[dt.i], " ");
            if (!trimmed_str)
                return (free_str_arr(new_arr), NULL);
            code = merge_splited_space(&new_arr, split_input_str(trimmed_str));
            my_free(trimmed_str);
        }
        if (code)
            return (free_str_arr(new_arr), NULL);
        dt.i++;
    }
    return (new_arr);
}

int polish_input_arr(char ***arr)
{
    char **new_arr;
    t_polish_data dt;

    dt = (t_polish_data){0};
    if (!arr || !*arr)
        return (1);
    dt.arr = ft_calloc(sizeof(char *), 1);
    if (dup_arr(&dt.arr, *arr))
        return (free_str_arr(new_arr), free_str_arr(dt.arr), 1);
    new_arr = split_space(dt);
    if (!new_arr)
        return (1);
    free_str_arr(dt.arr);
    dt.arr = ft_calloc(sizeof(char *), 1);
    if (dup_arr(&dt.arr, new_arr))
        return (free_str_arr(new_arr), free_str_arr(dt.arr), 1);
    if (final_merge(&new_arr, dt.arr))
        return (free_str_arr(new_arr), free_str_arr(dt.arr), 1);
    // free_str_arr(dt.arr);
    free_str_arr(*arr);
    *arr = new_arr;
    return (0);
}
