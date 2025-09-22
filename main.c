#include "main.h"

static void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    print_prompt_header();
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

static void setup_signals(void)
{
    // Handle Ctrl+C (SIGINT)
    signal(SIGINT, handle_sigint);
    // Ignore Ctrl+\ (SIGQUIT)
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
    char **args;

    if (ft_strncmp(input, "exit", 4) == 0)
    {
        free_shell_data(data);
        free(input);
        exit(0);
    }
    args = ft_split(input, ' ');
    if (!args)
        return;
    if (ft_strncmp(args[0], "$?", 2) == 0)
        return (ft_printf("%d: command not found\n", WEXITSTATUS(data->last_exit_status)), (void)0);
    exec_cmd_2(data, args[0], args);
    free_str_arr(args);
}

int main(int argc, char **argv, char **envp)
{    
    t_shell_data data;
    char *input;
    
    (void)argc; 
    (void)argv;
    setup_signals();
    init_shell_data(&data, envp);
    while (1)
    {
        input = read_input(&data);
        if (!input)
            break;
        if (*input)  // Only process non-empty input
        {
            add_history(input);  // Add to readline history
            process_input(&data, input);
            free(input);
        }
    }
    free_shell_data(&data);
    rl_clear_history();  // Clear readline history
    return (0);
}
