/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_iteration.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:38:49 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:42:33 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	prepare_pipe(int pipefd[2], t_cmd_node *cmd)
{
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (!cmd->is_pipe_out)
		return (0);
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

// in case of error closing pipes, sets to structure variables info about error
static int	handle_iter_error(t_exec_ctx *ctx, int pipefd[2], int input_fd)
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
	if (input_fd != -1)
		close(input_fd);
	ctx->prev_pipe_read = -1;
	ctx->had_error = true;
	return (1);
}

static int	handle_failed_command(t_shell_data *data, t_exec_ctx *ctx,
		t_cmd_node *cmd, int pipefd[2])
{
	if (ctx->prev_pipe_read != -1)
		close(ctx->prev_pipe_read);
	if (cmd->is_pipe_out)
	{
		if (pipefd[1] != -1)
			close(pipefd[1]);
		ctx->prev_pipe_read = pipefd[0];
	}
	else
	{
		if (pipefd[0] != -1)
			close(pipefd[0]);
		if (pipefd[1] != -1)
			close(pipefd[1]);
		ctx->prev_pipe_read = -1;
	}
	if (cmd == ctx->last_exec)
	{
		update_last_exit_status(data, 1);
		ctx->status_updated = true;
	}
	ctx->had_error = true;
	return (0);
}

static void	update_pipe_links(t_exec_ctx *ctx, t_cmd_node *cmd, int pipefd[2])
{
	if (cmd->is_pipe_out)
	{
		if (pipefd[1] != -1)
			close(pipefd[1]);
		ctx->prev_pipe_read = pipefd[0];
	}
	else
	{
		if (pipefd[0] != -1)
			close(pipefd[0]);
		if (pipefd[1] != -1)
			close(pipefd[1]);
		ctx->prev_pipe_read = -1;
	}
}

// pipe init, check for builtin,
int	process_command_node(t_shell_data *data, t_exec_ctx *ctx, t_cmd_node *cmd)
{
	int	pipefd[2];
	int	input_fd;
	int	parent_run;

	if (prepare_pipe(pipefd, cmd))
		return (handle_iter_error(ctx, pipefd, ctx->prev_pipe_read));
	if (cmd->failed_code != NO_FAIL)
		return (handle_failed_command(data, ctx, cmd, pipefd));
	input_fd = ctx->prev_pipe_read;
	parent_run = should_run_parent_builtin(cmd, ctx->exec_count);
	if (parent_run && run_parent_builtin(data, ctx, cmd))
		return (handle_iter_error(ctx, pipefd, input_fd));
	if (!parent_run && spawn_child_process(data, ctx, cmd, pipefd))
		return (handle_iter_error(ctx, pipefd, input_fd));
	if (input_fd != -1)
		close(input_fd);
	update_pipe_links(ctx, cmd, pipefd);
	return (0);
}
