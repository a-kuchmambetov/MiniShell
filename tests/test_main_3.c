#include "../main.h"

volatile sig_atomic_t g_signal_received = 0;

static void handle_sigint_prompt(int sig)
{
    (void)sig;
    g_signal_received = 1;
    write(STDOUT_FILENO, "\n", 1);
    print_prompt_header();
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

static void setup_signals_prompt(void)
{
    signal(SIGINT, handle_sigint_prompt);
    signal(SIGQUIT, SIG_IGN);
}

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
    printf("Index | Command | Args | Input Redirection | Output Redirection | Pipe In | Pipe Out | EOF word\n");
    while (current)
    {
        printf(COLOR_GREEN);
        printf("  %d   | %s | `%s` | %s | %s | %s | %s | `%s`\n", index++, current->cmd, current->args,
               t_redir_types[current->input_redir_type], t_redir_types[current->output_redir_type], t_pipe_types[current->is_pipe_in], t_pipe_types[current->is_pipe_out == 1 ? 2 : 0], current->input_redir_type == REDIR_HEREDOC ? current->input_redir : "");
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
    char *envp2[] = {"PATH=/home/artem/.nvm/versions/node/v22.19.0/bin:/home/artem/.local/funcheck/host:/home/artem/.npm-global/bin:/home/artem/bin:/home/artem/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin", "HOME=/home/artem", "PWD=/home/artem/minishell", NULL};
    parse_envp(&data, envp2);
    set_envp_from_env(&data);
    parse_exec_folders(&data);

    printf(COLOR_MAGENTA);
    printf("\n-------------------------------");
    printf(COLOR_RESET);
    printf(COLOR_RED);
    fflush(stdout);
    if (create_cmd_list(&data, arr))
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
    setup_signals_prompt();

    test("echo hello | grep h >> output.txt");
    test("   ls           -la       /home/user/Documents  > result.txt  <             input.txt                   ");
    test("cat << EOF | grep \"pattern >> output.txt\"");
    test("cat << \"EOF\"");
    test("cat << \'EOF\'");
    test("cat < Makefile \"  pattern >> output.txt\"");
    test("echo 'x -$PWD \"'$HOME'\" y -$PWD > output.txt'");
    test("cat 'test file.txt'; echo \"Hello World\"");
    test("cat<<'EOF'<Makefile < Test");
    test("echo 'x -$PWD \"'          $HOME       '\" y -$PWD > output.txt'");
    test("echo 'x -$PWD \"' || ><");
    test("echo 'x -$PWD \"' '|| ><'");
    test("echo 'x -$PWD \"' && '|| ><'");
    return 0;
}