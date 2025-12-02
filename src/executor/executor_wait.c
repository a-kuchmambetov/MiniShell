/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:39:19 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:39:20 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void print_coredump(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "Quit (core dumped)", 19);
}

int	wait_children(pid_t *pids, int count, pid_t last_child)
{
	int		status;
	int		last_status;
	int		i;
	pid_t	pid;

	last_status = 0;
	i = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, print_coredump);
	while (i < count)
	{
		status = 0;
		pid = waitpid(pids[i], &status, 0);
		if (g_signal_received == SIGQUIT)
			print_coredump(SIGQUIT);
		if (pid == last_child)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}
