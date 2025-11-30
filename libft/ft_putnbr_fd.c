/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-05 11:08:08 by akuchmam          #+#    #+#             */
/*   Updated: 2025-05-05 11:08:08 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	const char	digits[] = "0123456789";
	char		print_val[10];
	long		number;
	int			index;

	number = n;
	if (n == 0)
		return (ft_putchar_fd('0', fd));
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		number *= -1;
	}
	index = 0;
	while (number > 0)
	{
		print_val[index++] = digits[number % 10];
		number /= 10;
	}
	while (index--)
		ft_putchar_fd(print_val[index], fd);
}
