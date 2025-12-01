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
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	data;
	char			*input;

	(void)argc;
	(void)argv;
	setup_signals_prompt();
	init_shell_data(&data, envp);
	while (1)
	{
		input = readline(COLOR_CYAN "minishell > " COLOR_RESET);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			if (parse_input(&data, input) == 0 && data.cmd_list.first)
				execute_command_list(&data);
			free_cmd_list(&data.cmd_list);
			free(input);
		}
	}
	free_shell_data(&data);
	rl_clear_history();
	return (0);
}
