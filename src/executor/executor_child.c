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

static int	is_directory_exists(const char *path)
{
	struct stat	stats;

	if (path[0] != '.' && path[0] != '/')
		return (0);
	stat(path, &stats);
	if (S_ISDIR(stats.st_mode))
		return (1);
	return (0);
}

void	child_execute(t_shell_data *data, t_cmd_node *cmd, int prev_fd,
		int pipefd[2])
{
	int	status;

	if (setup_child_fds(cmd, prev_fd, pipefd) < 0)
		exit(1);
	if (!cmd->args)
		exit(1);
	if (is_directory_exists(cmd->args[0]))
	{
		ft_print_err("%s: Is a directory\n", cmd->args[0]);
		exit(126);
	}
	if (is_builtin(cmd->cmd))
	{
		status = exec_builtin(data, cmd->args);
		exit(status);
	}
	exec_external(data, cmd->args);
}

bool	should_run_parent_builtin(t_cmd_node *cmd, int exec_count)
{
	if (!cmd || is_empty_cmd(cmd))
		return (false);
	if (!is_builtin(cmd->cmd))
		return (false);
	if (exec_count != 1)
		return (false);
	if (cmd->is_pipe_in || cmd->is_pipe_out)
		return (false);
	return (true);
}
