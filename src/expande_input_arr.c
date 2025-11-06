#include "../main.h"

int expande_input_arr(t_shell_data *data, char **arr)
{
    char *input;
    int i;

    if (!data || !arr || !*arr)
        return (1);
    while (arr[i])
    {
        input = ft_strdup(arr[i]);
        if (!input)
            return (1);
        if (check_do_expansion(data->env_list, &input))
            return (free(input), 1);
        free(arr[i]);
        arr[i] = input;
        i++;
    }
    return (0);
}
