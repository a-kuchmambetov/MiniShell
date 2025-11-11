/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:45:00 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/29 19:45:00 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Removes outer quotes from a string (single or double), preserving spaces inside.
 */
char *strip_outer_quotes(const char *s)
{
	size_t len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len < 2)
		return (ft_strdup(s));
	if (len >= 2 && ((s[0] == '"' && s[len - 1] == '"')
			|| (s[0] == '\'' && s[len - 1] == '\'')))
		return (ft_substr(s, 1, len - 2)); // зберігає всі пробіли всередині
	return (ft_strdup(s));
}

/**
 * @brief Returns quote char if found at string start (' or ").
 */
char get_opening_quote(const char *s)
{
	if (!s || !*s)
		return (0);
	if (*s == '\'' || *s == '"')
		return (*s);
	return (0);
}

/**
 * @brief Concatenates tokens until closing quote found.
 */
void join_quoted_parts(char **res, char **args, int *i, char quote)
{
	char *tmp;

	while (args[*i + 1])
	{
		(*i)++;
		tmp = ft_strjoin(*res, " ");
		my_free(*res);
		*res = ft_strjoin(tmp, args[*i]);
		my_free(tmp);
		if (!*res)
			return;
		if (ft_strrchr(args[*i], quote))
			break;
	}
}

/**
 * @brief Collects the value part of "KEY=VALUE", merging quoted tokens if needed.
 */
char *collect_value_after_equal(char **args, int *i)
{
	char *start;
	char *res;
	char *clean;
	char quote;

	start = ft_strchr(args[*i], '=');
	if (!start)
		return (ft_strdup(""));

	start++; // move past '='
	// 🔸 1. Якщо після '=' ідуть лише пробіли → значення порожнє
	if (*start == '\0')
		return (ft_strdup(""));
	// 🔸 2. Якщо після '=' відкриваються лапки — беремо усе всередині лапок
	quote = get_opening_quote(start);
	res = ft_strdup(start);
	if (!res)
		return (NULL);
	// Якщо лапки відкриті, але не закриті — об'єднати решту токенів
	if (quote && !ft_strrchr(start + 1, quote))
		join_quoted_parts(&res, args, i, quote);
	clean = strip_outer_quotes(res);
	my_free(res);
	return (clean);
}

/**
 * @brief Builds "KEY=VALUE" pair for adding to env list.
 */
char *build_final_pair(char *name, char *clean)
{
	char *pair;
	char *final;

	pair = ft_strjoin(name, "=");
	if (!pair)
		return (NULL);
	final = ft_strjoin(pair, clean);
	my_free(pair);
	return (final);
}
