#include "../main.h"

int main()
{
    char **arr;
    char *input;

    input = "echo hello | grep h >> output.txt";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);

    t_shell_data data;
    data = (t_shell_data){0};

    if (create_cmd_list(&data.cmd_list, arr))
        return (ft_print_err("Error: Failed to create command list\n"), 1);

    t_cmd_list *cmd_list = &data.cmd_list;
    (void)cmd_list;
    free_shell_data(&data);
    free_str_arr(arr);
    return 0;
}