#include "../main.h"
#include "../src/parse_input_utils/parse_input_utils.h"

int test(t_shell_data data, char *input)
{
    char **arr;

    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    if (expand_input_arr(&data, arr))
    {
        free_str_arr(arr);
        return (ft_print_err("Error: Failed to expand input array\n"), 1);
    }
    if (polish_input_arr(&arr))
    {
        free_str_arr(arr);
        return (ft_print_err("Error: Failed to polish input array\n"), 1);
    }
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    ft_printf("\n\n");
    free_str_arr(arr);
    return 0;
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;

    t_shell_data data;
    data = (t_shell_data){0};

    // Check with empty envp
    char *envp2[] = {"PATH=/home/artem/bin:/bin", "HOME=/home/artem", "CMD=o hello world > res.txt", "TEST=something          inside my head", "COM= < Makefile'something             \"      head   3    \"        c", NULL};
    parse_envp(&data, envp2);
    if (data.env_list.len == 0)
    {
        ft_printf("Error: Failed to parse envp\n");
        return (1);
    }
    // parse_envp(&data, envp);
    if (set_envp_from_env(&data))
    {
        ft_printf("Error: Failed to set envp from env list\n");
        free_shell_data(&data);
        return (1);
    }
    parse_exec_folders(&data);
    if (data.envp == NULL)
    {
        ft_printf("Error: envp is NULL after setting from env list\n");
        free_shell_data(&data);
        return (1);
    }

    test(data, "<Makefile$TEST cat|grep something");
    test(data, "<Makefile $TEST\"dawdawd '      \" cat|grep something");
    test(data, "<Makefile     \"     $TEST        \" cat|grep something");
    test(data, "<Makefile\'$TEST\"         \"\' cat|grep something");
    test(data, "<Makefile'some' cat");
    test(data, "<< Makefile$TEST cat");
    test(data, "<< Makefile'some' cat");
    test(data, "ech$CMD");
    test(data, "$COM");
    test(data, "\"$COM\"");
    test(data, "'$COM'");

    free_shell_data(&data);
    return (data.last_exit_status);
}