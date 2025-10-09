#include "main.h"

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

char *read_input()
{
    char *input;

    print_prompt_header();
    input = readline("> ");
    if (!input)
        return (NULL);
    return (input);
}

void process_input(t_shell_data *data, char *input)
{
    (void)input;
    (void)data;
    // Empty function for now
    // Future implementation will parse and execute commands
}

int main(int argc, char **argv, char **envp)
{
    t_shell_data data;
    char *input;

    (void)argc;
    (void)argv;
    setup_signals_prompt();
    init_shell_data(&data, envp);
    while (1)
    {
        input = read_input();
        if (!input)
            break;
        if (*input) // Only process non-empty input
        {
            add_history(input); // Add to readline history
            process_input(&data, input);
            free(input);
        }
    }
    free_shell_data(&data);
    rl_clear_history(); // Clear readline history
    return (0);
}
