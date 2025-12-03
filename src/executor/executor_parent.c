/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:38:54 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:41:11 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	save_parent_stdio(int saved[2])
{
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] < 0 || saved[1] < 0)
	{
		if (saved[0] >= 0)
			close(saved[0]);
		if (saved[1] >= 0)
			close(saved[1]);
		perror("dup");
		return (-1);
	}
	return (0);
}

static void	restore_parent_stdio(int saved[2])
{
	if (saved[0] >= 0)
	{
		dup2(saved[0], STDIN_FILENO);
		close(saved[0]);
	}
	if (saved[1] >= 0)
	{
		dup2(saved[1], STDOUT_FILENO);
		close(saved[1]);
	}
}

static int	setup_parent_redirs(t_cmd_node *cmd, int saved[2], int pipefd[2])
{
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (save_parent_stdio(saved) < 0)
		return (-1);
	if (redirect_input(cmd, -1) < 0 || redirect_output(cmd, pipefd) < 0)
	{
		restore_parent_stdio(saved);
		return (-1);
	}
	return (0);
}

int	execute_builtin_parent(t_shell_data *data, t_cmd_node *cmd)
{
	int	saved[2];
	int	pipefd[2];
	int	status;

	if (setup_parent_redirs(cmd, saved, pipefd) < 0)
		return (-1);
	if (!cmd->args)
	{
		restore_parent_stdio(saved);
		return (-1);
	}
	status = exec_builtin_fd(data, cmd->args, saved);
	restore_parent_stdio(saved);
	update_last_exit_status(data, status);
	return (status);
}

int	run_parent_builtin(t_shell_data *data, t_exec_ctx *ctx, t_cmd_node *cmd)
{
	if (execute_builtin_parent(data, cmd) < 0)
		return (1);
	ctx->parent_handled_last = true;
	ctx->status_updated = true;
	return (0);
}
