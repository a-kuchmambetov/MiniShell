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

char *remove_quotes(const char *str)
{
    int len;
    char *res;

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
 * @brief Implements echo builtin with '-n' option and variable expansion
 * @return 0 on success, 1 on failure.
 */
int	builtin_echo(char **argv)
{
	int	i;
	int	no_newline;
	char *arg;
	char *next;

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
		if (argv[i + 1])
		{
			next = remove_quotes(argv[i + 1]);
			if (!next)
            {
                free(arg);
                return (1);
            }
			if (next[0] != '\0')
                write(1, " ", 1);
            free(next);
        }
		free(arg);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}
