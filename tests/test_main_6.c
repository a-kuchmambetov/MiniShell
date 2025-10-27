#include "../main.h"

int test(t_shell_data data, char *input)
{
    char **arr;

    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    arr = expande_input_arr(&data, arr);
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
    // char *envp2[] = {"PATH=/home/artem/.nvm/versions/node/v22.19.0/bin:/home/artem/.local/funcheck/host:/home/artem/.npm-global/bin:/home/artem/bin:/home/artem/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin", "HOME=/home/artem", NULL};
    // parse_envp(&data, envp2);
    parse_envp(&data, envp);
    set_envp_from_env(&data);
    parse_exec_folders(&data);

    test(data, "cat<<'EOF'<Makefile < Test");

    free_shell_data(&data);
    return (data.last_exit_status);
}