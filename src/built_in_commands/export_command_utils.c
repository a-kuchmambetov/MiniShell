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
 * @brief Removes outer quotes from a string, if present.
 *
 * Supports both single and double quotes. If only the opening
 * quote is found, removes it safely without crashing.
 *
 * @param s Input string.
 * @return Newly allocated unquoted string.
 */
char *strip_outer_quotes(const char *s)
{
<<<<<<< HEAD
	size_t len;
	char q;
=======
	size_t	len;
	char	q;
>>>>>>> f4d4f8a (export has ann issue with space)

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len < 2)
		return (ft_strdup(s));
	q = s[0];
	if ((q == '\'' || q == '"') && s[len - 1] == q)
<<<<<<< HEAD
		return (ft_substr(s, 1, len - 2)); // зберігає всі пробіли всередині
=======
		return (ft_substr(s, 1, len - 2));  // зберігає всі пробіли всередині
>>>>>>> f4d4f8a (export has ann issue with space)
	return (ft_strdup(s));
}

/**
 * @brief Returns quote char if found at string start (' or ").
 *
 * @param s Pointer to string.
 * @return Quote character, or 0 if none.
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
 * @brief Concatenates arguments until closing quote is found.
 *
 * @param res Pointer to result string (modified in-place).
 * @param args Command arguments array.
 * @param i Pointer to current index in args.
 * @param quote Opening quote character.
 */
void join_quoted_parts(char **res, char **args, int *i, char quote)
{
	char *tmp;

	while (args[*i + 1])
	{
		(*i)++;
		tmp = ft_strjoin(*res, " ");
		free(*res);
		*res = ft_strjoin(tmp, args[*i]);
		free(tmp);
		if (!*res)
			return;
		if (ft_strrchr(args[*i], quote))
			break;
	}
}

/**
 * @brief Collects the value part of "KEY=VALUE", merging quoted tokens.
 *
 * @param args Command arguments array.
 * @param i Pointer to current argument index.
 * @return Newly allocated value string.
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
	if (*start == '\0' || *start == ' ')
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
	free(res);
	return (clean);
}

/**
 * @brief Builds a "KEY=VALUE" string for environment storage.
 *
 * @param name Variable name.
 * @param clean Variable value without quotes.
 * @return Allocated "KEY=VALUE" string.
 */
char *build_final_pair(char *name, char *clean)
{
	char *pair;
	char *final;

	pair = ft_strjoin(name, "=");
	if (!pair)
		return (NULL);
	final = ft_strjoin(pair, clean);
	free(pair);
	return (final);
}
