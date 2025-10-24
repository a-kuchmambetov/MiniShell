#include "../main.h"

int main()
{
    char **arr;
    char *input;

    input = "echo hello | grep h >> output.txt";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "   ls           -la       /home/user/Documents  > result.txt  <             input.txt                   ";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "cat << EOF | grep \"pattern >> output.txt\"";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "cat < Makefile \"  pattern >> output.txt\"";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "cat < Makefile\"  pattern >> output.txt\"";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "echo 'x -$PWD \"'$HOME'\" y -$PWD > output.txt'";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "cat 'test file.txt'; echo \"Hello World\"";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "cat<<'EOF'<Makefile < Test";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "echo 'x -$PWD \"'          $HOME       '\" y -$PWD > output.txt'";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "echo 'x -$PWD \"' || ><";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "echo 'x -$PWD \"' '|| ><'";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    input = "echo 'x -$PWD \"' && '|| ><'";
    arr = split_input_str(input);
    if (!arr)
        return (ft_print_err("Error: Failed to split input string\n"), 1);
    ft_printf("INPUT:\n%s", input);
    ft_printf("\nRESULT:\n");
    for (int i = 0; arr[i]; i++)
        ft_printf("[%s] ", arr[i]);
    free_str_arr(arr);

    ft_printf("\n\n");

    return 0;
}