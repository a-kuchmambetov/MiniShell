/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:37:26 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/30 03:40:26 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	is_path(const char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

static int	is_directory_exists(const char *path)
{
	struct stat	st;

	if (!is_path(path))
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

void	child_execute(t_shell_data *data, t_cmd_node *cmd, int prev_fd,
		int pipefd[2])
{
	int	status;

	if (setup_child_fds(cmd, prev_fd, pipefd) < 0)
		my_exit(1, data);
	if (!cmd->args)
		my_exit(1, data);
	if (is_directory_exists(cmd->args[0]))
	{
		ft_print_err("%s: Is a directory\n", cmd->args[0]);
		my_exit(126, data);
	}
	if (is_builtin(cmd->cmd))
	{
		status = exec_builtin(data, cmd->args);
		my_exit(status, data);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	exec_external(data, cmd->args);
}

bool	should_run_parent_builtin(t_cmd_node *cmd, int exec_count)
{
	if (!cmd || is_empty_cmd(cmd))
		return (false);
	if (cmd->failed_code != NO_FAIL)
		return (false);
	if (!is_builtin(cmd->cmd))
		return (false);
	if (exec_count != 1)
		return (false);
	if (cmd->is_pipe_in || cmd->is_pipe_out)
		return (false);
	return (true);
}

// fork and call of child_execute
int	spawn_child_process(t_shell_data *data, t_exec_ctx *ctx, t_cmd_node *cmd,
		int pipefd[2])
{
	ctx->pids[ctx->child_count] = fork();
	if (ctx->pids[ctx->child_count] == 0)
	{
		my_free(ctx->pids);
		ctx->pids = NULL;
		child_execute(data, cmd, ctx->prev_pipe_read, pipefd);
	}
	if (ctx->pids[ctx->child_count] < 0)
	{
		perror("fork");
		return (1);
	}
	if (cmd == ctx->last_exec)
		ctx->last_child = ctx->pids[ctx->child_count];
	ctx->child_count++;
	return (0);
}
