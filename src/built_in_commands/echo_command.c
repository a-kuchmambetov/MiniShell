/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:57:56 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 12:58:56 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Checks whether a string is a valid echo -n flag.
 *
 * A valid flag must:
 *   - start with '-'
 *   - contain at least one character after '-'
 *   - consist only of 'n' characters (e.g. "-n", "-nnn")
 *
 * @param str Input string to check.
 * @return 1 if the string is a valid -n flag, 0 otherwise.
 */
static int	all_n(const char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

/**
 * @brief Removes surrounding single or double quotes from a string.
 *
 * If the string begins and ends with matching quotes ('...' or "..."),
 * a new allocated string without the outer quotes is returned.
 * If no surrounding quotes are found, a duplicate of the original
 * string is returned.
 *
 * @param str Input string.
 * @return Newly allocated string without quotes, or NULL on allocation failure.
 */
char	*remove_quotes(const char *str)
{
	char	*res;
	int		len;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		res = malloc(len - 1);
		if (!res)
			return (NULL);
		ft_strlcpy(res, str + 1, len - 1);
		return (res);
	}
	return (ft_strdup(str));
}

/**
 * @brief Implements the echo builtin with support for repeated "-n" flags.
 *
 * Handles:
 *   - one or multiple valid "-n" flags ("-n", "-nnn", "-nn")
 *   - printing arguments separated by spaces
 *   - optional suppression of the trailing newline
 *   - removal of surrounding quotes
 *
 * @param argv Array of arguments passed to the echo command.
 * @return 0 on success, 1 on allocation failure.
 */
int	builtin_echo(char **argv)
{
	char	*arg;
	int		i;
	int		no_newline;

	i = 1;
	no_newline = 0;
	while (argv[i] && all_n(argv[i]))
	{
		no_newline = 1;
		i++;
	}
	while (argv[i])
	{
		arg = remove_quotes(argv[i]);
		if (!arg)
			return (1);
		write(1, arg, strlen(arg));
		my_free(arg);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}
