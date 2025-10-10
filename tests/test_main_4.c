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

    char *filename;
    int result;

    // Test 1: Simple here-doc
    filename = NULL;
    result = start_here_doc("EOF", &filename);

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