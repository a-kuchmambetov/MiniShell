/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:51:50 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 13:14:31 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_tokens.h"

static char	*process_src(char *src, int in_quotes, int *check_token)
{
	if (!in_quotes)
	{
		if (*check_token == TOKEN_REDIR_IN || *check_token == TOKEN_REDIR_OUT
			|| *check_token == TOKEN_APPEND)
			if (is_ambig(src))
			{
				*check_token = TOKEN_AMBIGUOUS;
			}
		return (squash_spaces(src));
	}
	return (src);
}

static char	*get_next_src(t_env_list env, char *input, int *i, int *errno)
{
	if (input[*i] == '$')
		return (get_expanded_val(env, input + *i, i, errno));
	return (ft_strndup(input + *i, 1));
}

char	*process_expansion(t_env_list env, char *input, int *errno,
		int *current_type)
{
	char	*dest;
	char	*src;
	int		i;
	int		in_quotes;

	i = 0;
	in_quotes = (input[0] == '"');
	src = NULL;
	dest = ft_strdup("");
	if (!dest)
		return (NULL);
	while (input[i])
	{
		src = get_next_src(env, input, &i, errno);
		if (!src || *errno)
			return (my_free(dest), NULL);
		src = process_src(src, in_quotes, current_type);
		if (!src)
			return (my_free(dest), NULL);
		if (join_value(&dest, src, errno))
			return (my_free(dest), NULL);
		my_free(src);
		i++;
	}
	return (dest);
}

static char	*check_do_expansion(t_env_list env, char *input, int *errno,
		int *current_type)
{
	char	*result;

	if (!input)
	{
		*errno = 1;
		return (NULL);
	}
	if (input[0] == '\'')
		return (NULL);
	result = process_expansion(env, input, errno, current_type);
	if (*current_type == TOKEN_TEXT && result)
		*current_type = TOKEN_EXPANSION;
	return (result);
}

void	expand_tokens(t_token_list *tkn_li, t_shell_data *data, int *errno)
{
	t_token_node	*cur;
	int				cur_t;
	char			*src;

	cur = tkn_li->head;
	while (cur)
	{
		cur_t = cur->type;
		if (cur_t)
		{
			src = check_do_expansion(data->env_list, cur->value, errno, &cur_t);
			if (*errno)
				return (my_free(src), (void) NULL);
			if (src)
			{
				update_token(cur, src, cur_t, errno);
				if (*errno)
					return (my_free(src), (void) NULL);
				if (cur->type == TOKEN_AMBIGUOUS)
					ft_print_err("%s: ambiguous redirect\n", cur->value);
			}
		}
		cur = cur->next;
	}
	return ;
}
