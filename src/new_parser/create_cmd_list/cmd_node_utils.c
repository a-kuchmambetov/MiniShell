/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_node_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:50:22 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:50:23 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "create_cmd_list_utils.h"

void	push_cmd_node(t_cmd_list *cmd_list, t_cmd_node *new_node)
{
	t_cmd_node	*current;

	if (!cmd_list || !new_node)
		return ;
	current = cmd_list->first;
	if (!current)
	{
		cmd_list->first = new_node;
		cmd_list->len = 1;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = new_node;
	cmd_list->len++;
}

t_cmd_node	*malloc_cmd_node(int *errno)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	if (!node)
	{
		*errno = 1;
		return (NULL);
	}
	node->cmd = NULL;
	node->args = NULL;
	node->failed_code = NO_FAIL;
	node->input_redir_type = NO_REDIR;
	node->input_redir = NULL;
	node->output_redir_type = NO_REDIR;
	node->output_redir = NULL;
	node->is_pipe_in = 0;
	node->is_pipe_out = 0;
	node->next = NULL;
	return (node);
}
