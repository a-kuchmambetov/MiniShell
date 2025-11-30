/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-05 08:12:57 by akuchmam          #+#    #+#             */
/*   Updated: 2025-05-05 08:12:57 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_numlen(int n)
{
	int	num_len;

	num_len = 0;
	if (n < 0)
		num_len++;
	while (n != 0)
	{
		n /= 10;
		num_len++;
	}
	return (num_len);
}

static int	is_negative(int num)
{
	if (num < 0)
		return (1);
	return (0);
}

char	*ft_itoa(int n)
{
	const int	sign = is_negative(n);
	int			num_len;
	char		*str;

	num_len = count_numlen(n);
	if (n == 0)
		str = ft_calloc(1, 2);
	else
		str = ft_calloc(1, num_len + 1);
	if (!str)
		return (NULL);
	str[0] = '0';
	while (n)
	{
		if (n > 0)
			str[--num_len] = n % 10 + '0';
		else
			str[--num_len] = n % 10 * -1 + '0';
		n /= 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}
