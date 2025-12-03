/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:51:16 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:55:56 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_tokens.h"

int	join_value(char **dest, const char *src, int *errno)
{
	char	*new_str;

	if (!dest || !src)
	{
		*errno = 1;
		return (1);
	}
	if (!*dest)
		*dest = ft_calloc(1, 1);
	new_str = ft_strjoin(*dest, src);
	if (!new_str)
	{
		*errno = 1;
		return (1);
	}
	my_free(*dest);
	*dest = new_str;
	return (0);
}

char	*get_val_by_key(t_env_list env, const char *name)
{
	t_env_node	*current;
	size_t		len;
	char		*value;

	current = env.first;
	if (!name || !*name)
		return (NULL);
	len = ft_strlen(name);
	while (current)
	{
		if (!ft_strncmp(current->key, name, len))
		{
			value = ft_strdup(current->value);
			if (!value)
				return (NULL);
			return (value);
		}
		current = current->next;
	}
	return (NULL);
}

int	find_key_len(const char *str)
{
	int	len;

	len = 0;
	if (!str || !*str)
		return (0);
	if (str[len] == '?')
		return (1);
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

char	*get_expanded_val(t_env_list env, char *input, int *i, int *errno)
{
	const int	key_len = find_key_len(input + 1);
	char		*key;
	char		*env_value;

	if (key_len < 1)
	{
		env_value = ft_strdup("$");
		return (env_value);
	}
	key = ft_strndup(input + 1, key_len);
	if (!key)
	{
		*errno = 1;
		return (NULL);
	}
	env_value = get_val_by_key(env, key);
	if (!env_value)
	{
		env_value = ft_strdup("");
		if (!env_value)
			*errno = 1;
	}
	*i += key_len;
	return (my_free(key), env_value);
}

void	update_token(t_token_node *cur, char *new_value, int current_type,
	int *errno)
{
	if ((cur->type == TOKEN_REDIR_IN || cur->type == TOKEN_REDIR_OUT
			|| cur->type == TOKEN_APPEND) && ft_strncmp(new_value, "", 1) == 0)
		cur->type = TOKEN_AMBIGUOUS;
	else
		cur->type = current_type;
	if (cur->type == TOKEN_AMBIGUOUS)
		return ;
	my_free(cur->value);
	cur->value = ft_strdup(new_value);
	if (!cur->value)
		*errno = 1;
	my_free(new_value);
}
