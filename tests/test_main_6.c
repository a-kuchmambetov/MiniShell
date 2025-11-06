#include "../main.h"

int test(t_shell_data data, char *input)
{
    char **arr;

    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    expande_input_arr(&data, arr);
    // split_space(&arr);
    polish_input_arr(&arr);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");
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
    char *envp2[] = {"PATH=/home/artem/bin:/bin", "HOME=/home/artem", "CMD=o hello world > res.txt", "TEST=something inside my head", "COM= < Makefile'something'           c", NULL};
    parse_envp(&data, envp2);
    // parse_envp(&data, envp);
    set_envp_from_env(&data);
    parse_exec_folders(&data);

    test(data, "<Makefile$TEST cat");
    test(data, "<Makefile'some' cat");
    test(data, "<< Makefile$TEST cat");
    test(data, "<< Makefile'some' cat");
    test(data, "ech$CMD");
    test(data, "$COM");
    test(data, "'$COM'");

    int str_len = ft_strlen(" ");
    (void)str_len;

    free_shell_data(&data);
    return (data.last_exit_status);
}