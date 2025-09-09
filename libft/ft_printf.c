/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-02 10:20:01 by akuchmam          #+#    #+#             */
/*   Updated: 2025-07-02 10:20:01 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_handle_format(char c, va_list *args)
{
	if (c == 'c')
		return (ft_putchar(va_arg(*args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(*args, char *)));
	else if (c == 'p')
		return (ft_putptr(va_arg(*args, void *)));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr_base(va_arg(*args, int), "0123456789"));
	else if (c == 'u')
		return (ft_putnbr_unsigned(va_arg(*args, unsigned int), "0123456789"));
	else if (c == 'x')
		return (ft_putnbr_unsigned(va_arg(*args, unsigned int),
				"0123456789abcdef"));
	else if (c == 'X')
		return (ft_putnbr_unsigned(va_arg(*args, unsigned int),
				"0123456789ABCDEF"));
	else if (c == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;
	int		i;

	if (!format)
		return (-1);
	va_start(args, format);
	count = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			count += ft_handle_format(format[i], &args);
		}
		else if (format[i] != '%')
			count += ft_putchar(format[i]);
		i++;
	}
	va_end(args);
	return (count);
}
