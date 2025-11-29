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

int	wait_children(pid_t *pids, int count, pid_t last_child)
{
	int		status;
	int		last_status;
	int		i;
	pid_t	pid;

	last_status = 0;
	i = 0;
	while (i < count)
	{
		status = 0;
		pid = waitpid(pids[i], &status, 0);
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
