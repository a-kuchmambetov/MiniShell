/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:47:38 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 13:47:39 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Checks if a string is a valid shell identifier.
 *
 * A valid identifier starts with a letter or underscore ('_') and
 * may contain letters, digits, or underscores. Stops checking at '='.
 *
 * @param str The string to validate.
 * @return 1 if the identifier is valid, 0 otherwise.
 */
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Checks if a number is out of long range.
 * @param neg Sign of the number (1 or -1).
 * @param num The numeric value to check.
 * @param error Pointer to error flag, set to true if out of range.
 * @return true if out of range, false otherwise.
 */
static bool	check_out_of_range(int neg, unsigned long long num, bool *error)
{
	if (neg == 1)
	{
		if (num > LONG_MAX)
			*error = true;
	}
	else
	{
		if (num > -(unsigned long)LONG_MIN)
			*error = true;
	}
	return (*error);
}

/**
 * @brief Converts a string to long with overflow detection.
 * @param str Input string.
 * @param error Pointer to overflow error flag.
 * @return Converted long value.
 */
long	ft_atoi_long(const char *str, bool *overflow)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	neg = 1;
	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			neg = -1;
	while (str[i] && ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		if (check_out_of_range(neg, num, overflow))
			break ;
		i++;
	}
	return ((long)(num * neg));
}
