/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:50:12 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 13:16:58 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "create_cmd_list_utils.h"

static int	handle_word(t_cmd_node *node, t_token_node *tkn, int *errno)
{
	if (!tkn->value || *(tkn->value) == '\0')
		return (0);
	if (!node->cmd)
	{
		node->cmd = ft_strdup(tkn->value);
		if (!node->cmd)
			return (*errno = 1);
		append_arg(node, node->cmd, errno);
		if (*errno)
			return (1);
		return (0);
	}
	return (append_arg(node, tkn->value, errno));
}

static int	handle_input_redir(t_shell_data *dt, t_cmd_node *node,
		t_token_node *tkn, int *errno)
{
	if (tkn->type == TOKEN_HEREDOC || tkn->type == TOKEN_HEREDOC_QUOTES)
	{
		my_free(node->input_redir);
		node->input_redir = NULL;
		node->input_redir_type = REDIR_HEREDOC;
		if (start_here_doc(dt->env_list, tkn->value,
				&node->input_redir, tkn->type))
			return (set_errno(1, errno));
	}
	else
	{
		if (node->failed_code != NO_FAIL)
			return (0);
		my_free(node->input_redir);
		node->input_redir = NULL;
		node->input_redir_type = REDIR_INPUT;
		node->input_redir = ft_strdup(tkn->value);
		if (!node->input_redir)
			return (set_errno(1, errno));
	}
	if (node->input_redir_type == REDIR_INPUT
		&& check_file_open(node->input_redir) == 1)
		node->failed_code = FAILED_IN;
	return (0);
}

static int	handle_output_redir(t_cmd_node *node, t_token_node *tkn, int *errno)
{
	if (node->failed_code != NO_FAIL)
		return (0);
	if (node->output_redir)
	{
		my_free(node->output_redir);
		node->output_redir = NULL;
	}
	if (tkn->type == TOKEN_APPEND)
		node->output_redir_type = REDIR_APPEND;
	else
		node->output_redir_type = REDIR_OUTPUT;
	node->output_redir = ft_strdup(tkn->value);
	if (!node->output_redir)
		return (set_errno(1, errno));
	if (node->output_redir && check_file_create(node->output_redir,
			node->output_redir_type) == 1)
	{
		node->failed_code = FAILED_OUT;
		return (0);
	}
	return (0);
}

static int	process_token(t_shell_data *dt, t_token_node *tkn,
		t_cmd_node **node, int *errno)
{
	if (!tkn)
		return (0);
	if (tkn->type == TOKEN_AMBIGUOUS)
	{
		(*node)->failed_code = FAILED_AMBIG;
		return (0);
	}
	if (tkn->type == TOKEN_PIPE)
	{
		(*node)->is_pipe_out = 1;
		push_cmd_node(&dt->cmd_list, *node);
		*node = malloc_cmd_node(errno);
		if (*errno)
			return (1);
		(*node)->is_pipe_in = 1;
		return (0);
	}
	if (tkn->type == TOKEN_REDIR_IN || tkn->type == TOKEN_HEREDOC
		|| tkn->type == TOKEN_HEREDOC_QUOTES)
		return (handle_input_redir(dt, *node, tkn, errno));
	if (tkn->type == TOKEN_REDIR_OUT || tkn->type == TOKEN_APPEND)
		return (handle_output_redir(*node, tkn, errno));
	return (handle_word(*node, tkn, errno));
}

int	create_cmd_list(t_shell_data *dt, t_token_list *tkn_li, int *errno)
{
	t_token_node	*cur;
	t_cmd_node		*node;

	if (!dt || !tkn_li || !errno)
		return (1);
	*errno = 0;
	node = malloc_cmd_node(errno);
	if (*errno)
		return (1);
	cur = tkn_li->head;
	if (!cur)
		return (free_cmd_node(node), 1);
	while (cur)
	{
		process_token(dt, cur, &node, errno);
		cur = cur->next;
	}
	if (*errno)
		return (free_cmd_node(node), free_cmd_list(&dt->cmd_list), 1);
	if (has_content(node))
		push_cmd_node(&dt->cmd_list, node);
	else
		free_cmd_node(node);
	return (0);
}
