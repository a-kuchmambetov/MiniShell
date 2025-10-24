#include "../main.h"

// 1. Iterate through each string in the array 'arr'.
// 2. For each string do expansion.
// 3. Split expanded string.
// 4. Add new splited array with old arr to new extended array.
// 5. Free the original array and point 'arr' to the new array.

int count_arr_len(char **arr)
{
    int len;

    len = 0;
    while (arr[len])
        len++;
    return (len);
}

// int c

char **create_new_array(char **arr, char **new_arr, int *i)
{
    char **extended_arr;
    int i_dest;
    int i_src;

    i_dest = 0;
    i_src = 0;
    extended_arr = ft_calloc(sizeof(char *), *i);
    if (!extended_arr)
        return (NULL);
    while (arr[i_dest])
    {
        if (i_src == *i)
        {
            int j = 0;
            while (new_arr[j])
            {
                extended_arr[i_dest] = ft_strdup(new_arr[j]);
                i_dest++;
                j++;
            }
            i_src++;
        }
        else
            extended_arr[i_dest] = ft_strdup(arr[i_src]);
        i_src++;
    }
    return (extended_arr);
}

char **expande_input_str(t_shell_data *data, char **arr, int i)
{
    char *input;
    // char **new_arr;

    input = ft_strdup(arr[i]);
    if (!input)
        return (NULL);
    if (process_expansion(data->env_list, &input))
        return (free(input), NULL);
    // if (input[0] == ' ' && arr[i][0] == '$')
    //     replace_curr_str();
    // else
    //     append_prev_str();
    free(input);
    return (NULL);
    // return (new_arr);
}

char **expande_input_arr(t_shell_data *data, char **arr)
{
    char **new_arr;
    int i;

    if (!data || !arr || !*arr)
        return (NULL);
    while ((*arr)[i])
    {
        new_arr = expande_input_str(data, arr, i);
        if (!new_arr)
            return (NULL);
        i++;
    }
    return (new_arr);
}
