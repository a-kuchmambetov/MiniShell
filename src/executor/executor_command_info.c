/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_info.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:37:36 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:37:38 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	is_empty_cmd(t_cmd_node *cmd)
{
	if (!cmd)
		return (true);
	if (cmd->failed_code != NO_FAIL)
		return (false);
	if (!cmd->cmd || cmd->cmd[0] == ' ')
		return (true);
	return (cmd->cmd[0] == '\0');
}

bool	is_question_command(t_cmd_node *cmd)
{
	if (!cmd || !cmd->cmd)
		return (false);
	return (ft_strncmp(cmd->cmd, "$?", 3) == 0);
}

int	count_executable(t_cmd_node *node)
{
	int	count;

	count = 0;
	while (node)
	{
		if (!is_empty_cmd(node))
			count++;
		node = node->next;
	}
	return (count);
}

t_cmd_node	*find_last_executable(t_cmd_node *node)
{
	t_cmd_node	*last;

	last = NULL;
	while (node)
	{
		if (!is_empty_cmd(node))
			last = node;
		node = node->next;
	}
	return (last);
}

void	cleanup_heredocs(t_cmd_node *cmd)
{
	while (cmd)
	{
		if (cmd->input_redir_type == REDIR_HEREDOC && cmd->input_redir)
			delete_here_doc(cmd->input_redir);
		cmd = cmd->next;
	}
}
