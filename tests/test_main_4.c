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

int main()
{
    setup_signals_prompt();
    t_shell_data data;
    data = (t_shell_data){0};
    char *envp2[] = {"PATH=/home/artem/.nvm/versions/node/v22.19.0/bin:/home/artem/.local/funcheck/host:/home/artem/.npm-global/bin:/home/artem/bin:/home/artem/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin", "HOME=/home/artem", NULL};
    parse_envp(&data, envp2);
    set_envp_from_env(&data);
    parse_exec_folders(&data);

    char *filename;
    int result;

    // Test 1: Simple here-doc
    filename = NULL;
    result = start_here_doc(data.env_list, "EOF", &filename);

    if (result == 0 && filename)
    {
        printf("Here-doc created: %s\n", filename);
        // Use the file...

        // Clean up
        // delete_here_doc(filename);
        free(filename);
    }
    else if (result == 130)
        printf("Here-doc interrupted by signal\n");
}