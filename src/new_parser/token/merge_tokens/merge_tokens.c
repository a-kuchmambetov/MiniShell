/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:15 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:52:16 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "merge_tokens.h"

static int	is_next_mergeable(t_token_node *cur)
{
	if (cur->next == NULL)
		return (0);
	if (cur->is_space_after)
		return (0);
	if (cur->type == TOKEN_PIPE || cur->next->type == TOKEN_PIPE
		|| cur->next->type == TOKEN_HEREDOC
		|| cur->next->type == TOKEN_HEREDOC_QUOTES
		|| cur->next->type == TOKEN_REDIR_IN
		|| cur->next->type == TOKEN_REDIR_OUT || cur->next->type == TOKEN_APPEND
		|| cur->next->type == TOKEN_AMBIGUOUS)
		return (0);
	return (1);
}

static int	is_space_after(char *str)
{
	if (!str || ft_strlen(str) == 0)
		return (0);
	if (str[ft_strlen(str) - 1] == ' ')
		return (1);
	return (0);
}

static char	*join_value(char *val1, char *val2, int *errno)
{
	const int	is_quoted_1 = is_quoted(val1);
	const int	is_quoted_2 = is_quoted(val2);
	char		*new_value;

	if (!is_quoted_1)
		val1 = trim_space_before(val1);
	if (!is_quoted_2 && is_space_after(val1))
		val2 = trim_space_before(val2);
	if (is_quoted_1)
		trim_quotes(val1);
	if (is_quoted_2)
		trim_quotes(val2);
	new_value = ft_strjoin(val1, val2);
	if (!new_value)
	{
		*errno = 1;
		return (NULL);
	}
	if (!is_quoted_2)
		new_value = trim_space_after(new_value);
	return (new_value);
}

void	merge_tokens(t_token_list tkn_li, int *errno)
{
	t_token_node	*cur;
	t_token_node	*next_free;
	char			*temp;

	cur = tkn_li.head;
	while (cur)
	{
		if (is_next_mergeable(cur))
		{
			temp = join_value(cur->value, cur->next->value, errno);
			if (*errno)
				return ;
			free(cur->value);
			cur->is_space_after = cur->next->is_space_after;
			cur->value = temp;
			next_free = cur->next;
			cur->next = cur->next->next;
			free_token_node(next_free);
		}
		else
		{
			cur->value = trim_quotes(cur->value);
			cur = cur->next;
		}
	}
}
