/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-02 10:19:52 by akuchmam          #+#    #+#             */
/*   Updated: 2025-07-02 10:19:52 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *s)
{
	int	count;

	if (!s)
	{
		write(1, "(null)", 6);
		return (6);
	}
	count = write(1, s, ft_strlen(s));
	return (count);
}

int	ft_putnbr_base(long n, char *base)
{
	int	count;
	int	base_len;

	count = 0;
	base_len = ft_strlen(base);
	if (n < 0)
	{
		count += ft_putchar('-');
		n = -n;
	}
	if (n >= base_len)
		count += ft_putnbr_base(n / base_len, base);
	count += ft_putchar(base[n % base_len]);
	return (count);
}

int	ft_putnbr_unsigned(unsigned long n, char *base)
{
	int	count;
	int	base_len;

	count = 0;
	base_len = ft_strlen(base);
	if (n >= (unsigned long)base_len)
		count += ft_putnbr_unsigned(n / base_len, base);
	count += ft_putchar(base[n % base_len]);
	return (count);
}

int	ft_putptr(void *ptr)
{
	unsigned long	addr;
	int				count;

	if (!ptr)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	addr = (unsigned long)ptr;
	count = ft_putstr("0x");
	count += ft_putnbr_unsigned(addr, "0123456789abcdef");
	return (count);
}
