#include "../main.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	handle_sigint_prompt(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	setup_signals_prompt(void)
{
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

int test (char *input, char **envp)
{
    t_shell_data data;
    data = (t_shell_data){0};
    init_shell_data(&data, envp);

    printf("Running %s: \n\n", input);
    if (parse_input(&data, input) == 0 && data.cmd_list.first)
        execute_command_list(&data);
    printf("Last exit status: %d\n\n", data.last_exit_status);
    free_shell_data(&data);
    return 0;
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;(void)argv;(void)envp;
	setup_signals_prompt();
    // Check with empty envp
    char *envp2[] = { "PATH=/bin", NULL };
    (void) envp2;
    printf("\nTRYING TO EXECUTE:\n");

    test("ls -la", envp);
    test("gcc --version", envp);
    test("nonexistentcommand", envp);
    test("echo \"Hello World\"", envp);
    test("cat test file.txt", envp);
    test("cat \"test file.txt\"", envp);
    test("   ls           -la       /  > result.txt  <             Makefile                   ; cat result.txt", envp);

    return 0;
}