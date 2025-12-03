/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 03:29:11 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/30 03:29:35 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
	if (isatty(STDIN_FILENO))
	{
		signal(SIGINT, handle_sigint_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
}

static char	*read_line_prompt(void)
{
	if (isatty(STDIN_FILENO))
		return ("minishell > ");
	else
		return (" > ");
}

static void	finish_cmd(t_shell_data *data, char *input)
{
	if (isatty(STDIN_FILENO))
		ft_printf("\n");
	free_cmd_list(&data->cmd_list);
	my_free(input);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	data;
	char			*input;

	(void)argc, (void)argv;
	init_shell_data(&data, envp);
	while (1)
	{
		setup_signals_prompt();
		input = readline(read_line_prompt());
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 5);
			break ;
		}
		if (*input)
		{
			add_history(input);
			if (parse_input(&data, input) == 0 && data.cmd_list.first)
				execute_command_list(&data);
			finish_cmd(&data, input);
		}
	}
	free_shell_data(&data);
	rl_clear_history();
}
