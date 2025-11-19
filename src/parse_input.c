#include "../main.h"
#include "parse_input_utils/parse_input_utils.h"

int parse_input(t_shell_data *data, char *input)
{
    char **arr;

    if (data->cmd_list.first)
        free_cmd_list(&data->cmd_list);
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    if (expand_input_arr(data, arr))
    {
        free_str_arr(arr);
        return (ft_print_err("Error: Failed to expand input array\n"), 1);
    }
    if (polish_input_arr(&arr))
    {
        free_str_arr(arr);
        return (ft_print_err("Error: Failed to polish input array\n"), 1);
    }
    if (create_cmd_list(data, arr))
    {
        free_str_arr(arr);
        return (ft_print_err("Error: Failed to create command list\n"), 1);
    }
    free_str_arr(arr);
    return (0);
}
