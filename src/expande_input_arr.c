#include "../main.h"

// 1. Iterate through each string in the array 'arr'.
// 2. For each string do expansion.
// 3. Split expanded string.
// 4. Add new splited array with old arr to new extended array.
// 5. Free the original array and point 'arr' to the new array.

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
        if (process_expansion(data->env_list, &input))
            return (free(input), 1);
        free(arr[i]);
        arr[i] = input;
        i++;
    }
    return (0);
}
