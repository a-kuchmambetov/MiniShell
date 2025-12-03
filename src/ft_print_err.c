/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_err.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:43 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:52:44 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

static char	*join_strs(const char *dest, const char *src)
{
	const size_t	len1 = ft_strlen(dest);
	const size_t	len2 = ft_strlen(src);
	char			*result;

	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, dest, len1);
	ft_memcpy(result + len1, src, len2);
	result[len1 + len2] = '\0';
	my_free((void *)dest);
	return (result);
}

static char	*pr_putnbr_base(long n, char *base)
{
	int		base_len;
	char	*str;
	char	*temp;

	str = ft_strdup("");
	if (!str)
		return (NULL);
	base_len = ft_strlen(base);
	if (n < 0)
	{
		str = join_strs(str, "-");
		if (!str)
			return (NULL);
		n = -n;
	}
	if (n >= base_len)
		str = join_strs(str, pr_putnbr_base(n / base_len, base));
	if (!str)
		return (NULL);
	temp = (char [2]){base[n % base_len], '\0'};
	str = join_strs(str, temp);
	if (!str)
		return (NULL);
	return (str);
}

static char	*process_print(char *dest, const char *format, int *i, va_list args)
{
	char	*temp;

	if (format[*i] == '%')
	{
		if (format[++(*i)] == 's')
			dest = join_strs(dest, va_arg(args, char *));
		else if (format[*i] == 'd')
		{
			temp = pr_putnbr_base(va_arg(args, int), "0123456789");
			dest = join_strs(dest, temp);
			my_free(temp);
		}
	}
	else
	{
		temp = (char [2]){format[*i], '\0'};
		dest = join_strs(dest, temp);
	}
	return (dest);
}

int	ft_print_err(const char *format, ...)
{
	int		i;
	va_list	args;
	char	*str;

	str = ft_strdup("minishell: ");
	if (!str)
		return (1);
	va_start(args, format);
	i = 0;
	while (format[i])
	{
		str = process_print(str, format, &i, args);
		if (!str)
			return (1);
		i++;
	}
	va_end(args);
	if (!str)
		return (1);
	write(2, str, ft_strlen(str));
	my_free(str);
	return (0);
}
