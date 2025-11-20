#include "../main.h"

int test(t_shell_data data, char *input)
{
    char **arr;
    int errno;
    t_token_list *token_list;

    arr = split_input(input, &errno);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    token_list = create_token_list(arr, &errno);
    if (errno)
    {
        free_str_arr(arr);
        return (ft_print_err("Error: Failed to create token list\n"), 1);
    }
    expand_tokens(token_list, &data, &errno);
    if (errno)
    {
        free_str_arr(arr);
        return (ft_print_err("Error: Failed to expand input array\n"), 1);
    }
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    t_token_node *current = token_list->head;
    while (current)
    {
        ft_printf("[%s] ", current->value);
        current = current->next;
    }
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
    char *envp2[] = {"PATH=/home/artem/bin:/bin", "HOME=/home/artem", "CMD=o            hello              world > res.txt", "TEST=something          inside my head", "COM= < Makefile'something             \"      head   3    \"        c", NULL};
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

    // test(data, "<Makefile$TEST cat|grep something");
    // test(data, "<Makefile $TEST\"dawdawd '      \" cat|grep something");
    // test(data, "<Makefile     \"     $TEST        \" cat|grep something");
    // test(data, "<Makefile\'$TEST\"         \"\' cat|grep something");
    // test(data, "<Makefile'some' cat");
    // test(data, "<< Makefile$TEST cat");
    // test(data, "<< Makefile'some' cat");
    // test(data, "ech$");
    test(data, "ech$CMD");
    test(data, "ech\"$CMD\"");
    // test(data, "$COM");
    // test(data, "\"$COM\"");
    // test(data, "'$COM'");

    free_shell_data(&data);
    return (data.last_exit_status);
}