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

int main(int argc, char **argv, char **envp)
{
    t_shell_data	data;

	(void)argc; (void)argv;
	setup_signals_prompt();
	init_shell_data(&data, envp);

    char **test_inputs = (char *[]){
        "echo hello some text and other stuff | grep h >> output.txt",
        "   ls           -la       /home/user/Documents  > result.txt  <             input.txt                   ",
        // "cat << EOF | grep \"pattern >> output.txt\"",
        // "cat << \"EOF\"",
        // "cat << \'EOF\'",
        NULL
    };
    int i = 0;
	while (test_inputs[i])
	{
            ft_printf("\nExecuting test input: %s\n", test_inputs[i]);
			if (parse_input(&data, test_inputs[i]) == 0 && data.cmd_list.first)
				execute_command_list(&data);
			free_cmd_list(&data.cmd_list);
            i++;
	}
	free_shell_data(&data);
	printf("All tests completed.\n");
    return 0;
}
