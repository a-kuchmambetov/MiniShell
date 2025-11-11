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
 * @brief Checks if string consists only of 'n' characters.
 * @param str Input string.
 * @return 1 if only 'n', 0 otherwise.
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
 * @brief Implements echo builtin with '-n' option and variable expansion.
 * @param argv Command arguments.
 * @param envp Environment variables.
 * @return 0 on success, 1 on failure.
 */
int	builtin_echo(char **argv, char **envp)
{
	int		i;
	int		no_newline;
	char	**split_args;
	char	*arg;

	if (argv[1] && ft_strchr(argv[1], ' '))
		split_args = ft_split(argv[1], ' ');
	else
		split_args = &argv[1];
	i = 0;
	no_newline = 0;
	while (split_args[i] && all_n(split_args[i]))
	{
		no_newline = 1;
		i++;
	}
	while (split_args[i])
	{
		arg = parse_arg(split_args[i], envp);
		if (!arg)
			return (1);
		write(1, arg, ft_strlen(arg));
		free(arg);
		if (split_args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	if (argv[1] && ft_strchr(argv[1], ' '))
		free_str_arr(split_args);
	return (0);
}
