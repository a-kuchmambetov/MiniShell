#include "main.h"

char *create_prompt()
{
    const char *cwd = getcwd(NULL, 0);
    char *prompt;

    if (!cwd)
        return (NULL);
    prompt = ft_calloc(1, ft_strlen(cwd) + 20);
    if (!prompt)
        return (NULL);
    ft_strlcpy(prompt, COLOR_CYAN, ft_strlen(COLOR_CYAN) + 1);
    ft_strlcat(prompt, cwd, ft_strlen(prompt) + ft_strlen(cwd) + 1);
    ft_strlcat(prompt, " \n> \033[0m", ft_strlen(prompt) + 10);
    free((char *)cwd);
    return (prompt);
}

static void handle_sigint(int sig)
{
    char *prompt;

    (void)sig;
    prompt = create_prompt();
    ft_printf("\n%s", prompt);
    free(prompt);
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
    char *prompt;
    char *input;

    prompt = create_prompt();
    if (!prompt)
        return (NULL);
    input = readline(prompt);
    free(prompt);
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

void init_shell_data(t_shell_data *data, char **envp)
{
    char *cwd;

    *data = (t_shell_data){0};
    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (free_shell_data(data), (void)0);
    if (data->pwd)
        free(data->pwd);
    data->pwd = ft_strdup(cwd);
    free(cwd);
    parse_envp(data, envp);
    parse_exec_folders(data);
    set_envp_from_env(data);
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
        }
        free(input);
    }
    free_shell_data(&data);
    rl_clear_history();  // Clear readline history
    return (0);
}
