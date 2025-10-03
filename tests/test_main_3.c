#include "../main.h"

int print_cmd_list(t_cmd_list *cmd_list, char *input)
{
    const char **t_redir_types = (const char *[]){"No redir", "input", "output", "append", "heredoc", "pipe"};
    const char **t_pipe_types = (const char *[]){"_", "pipe in", "pipe out"};

    t_cmd_node *current = cmd_list->first;
    int index = 0;

    printf(COLOR_CYAN);
    printf("\nCommand List (Total Commands: %d):\n", cmd_list->len);
    printf(COLOR_RESET);
    printf("Input: %s%s\n", COLOR_YELLOW, input);
    printf(COLOR_RESET);
    printf("-------------------------------\n");
    printf("Index | Command | Args | Input Redirection | Output Redirection | Pipe In | Pipe Out\n");
    while (current)
    {
        printf(COLOR_GREEN);
        printf("  %d   | %s | `%s` | %s | %s | %s | %s\n", index++, current->cmd, current->args,
               t_redir_types[current->input_redir_type], t_redir_types[current->output_redir_type], t_pipe_types[current->is_pipe_in], t_pipe_types[current->is_pipe_out == 1 ? 2 : 0]);
        current = current->next;
        printf(COLOR_RESET);
    }
    printf("-------------------------------\n");
    return 0;
}

int test(char *input)
{
    char **arr;

    arr = split_input_str(input);
    if (!arr)
    {
        printf(COLOR_CYAN);
        printf("\nCommand List (Total Commands: %d):\n", 0);
        printf(COLOR_RESET);
        printf("Input: %s%s\n", COLOR_YELLOW, input);
        printf(COLOR_RESET);
        printf("-------------------------------\n");
        fflush(stdout);
        printf(COLOR_RED);
        ft_print_err("Error: Failed to split input string\n");
        printf(COLOR_RESET);
        fflush(stdout);
        printf("-------------------------------\n");
        free_str_arr(arr);
        return (1);
    }
    fflush(stdout);

    t_shell_data data;
    data = (t_shell_data){0};

    printf(COLOR_MAGENTA);
    printf("\n-------------------------------");
    printf(COLOR_RESET);
    printf(COLOR_RED);
    fflush(stdout);
    if (create_cmd_list(&data.cmd_list, arr))
    {
        printf(COLOR_RESET);
        printf(COLOR_CYAN);
        printf("Command List (Total Commands: %d):\n", 0);
        printf(COLOR_RESET);
        printf("Input: %s%s\n", COLOR_YELLOW, input);
        printf(COLOR_RESET);
        fflush(stdout);
        printf("-------------------------------\n");
        fflush(stdout);
        free_shell_data(&data);
        free_str_arr(arr);
        return (1);
    }

    t_cmd_list *cmd_list = &data.cmd_list;
    fflush(stdout);
    print_cmd_list(cmd_list, input);
    fflush(stdout);

    free_shell_data(&data);
    free_str_arr(arr);
    return 0;
}

int main()
{
    test("echo hello | grep h >> output.txt");
    test("   ls           -la       /home/user/Documents  > result.txt  <             input.txt                   ");
    test("cat << EOF | grep \"pattern >> output.txt\"");
    test("cat < Makefile \"  pattern >> output.txt\"");
    test("echo 'x -$PWD \"'$HOME'\" y -$PWD > output.txt'");
    test("cat 'test file.txt'; echo \"Hello World\"");
    test("cat<<'EOF'<Makefile < Test");
    test("echo 'x -$PWD \"'          $HOME       '\" y -$PWD > output.txt'");
    test("echo 'x -$PWD \"' || ><");
    test("echo 'x -$PWD \"' '|| ><'");
    test("echo 'x -$PWD \"' && '|| ><'");
    // test("cat < file.txt");
    // test("grep 'pattern' < input.txt > output.txt");
    // test("sort < unsorted.txt | uniq > sorted_unique.txt");
    // test("echo 'New Entry' >> log.txt");
    // test("cat < input.txt | grep 'search' >> results.txt");
    // test("ls -l | grep '^d' > directories.txt");
    return 0;
}