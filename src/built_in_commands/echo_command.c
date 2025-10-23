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
int	all_n(char *str)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (str[j] != 'n')
			return (0);
		j++;
	}
	return (1);
}

/**
 * @brief Skips all '-n' flags in echo arguments.
 * @param argv Command arguments.
 * @return Index of the first non-flag argument.
 */
static int	skip_n_flags(char **argv)
{
	int	i;

	i = 1;
	while (argv[i] && !ft_strncmp(argv[i], "-n", 2))
	{
		if (!all_n(argv[i] + 1))
			break ;
		i++;
	}
	return (i);
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
	int		n_flag;
	char	*arg;

	i = skip_n_flags(argv);
	n_flag = (i > 1);
	while (argv[i])
	{
		arg = parse_arg(argv[i], envp);
		if (!arg)
			return (1);
		write(1, arg, ft_strlen(arg));
		free(arg);
		if (argv[++i])
			write(1, " ", 1);
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}
