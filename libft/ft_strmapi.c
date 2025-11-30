/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-05 10:41:10 by akuchmam          #+#    #+#             */
/*   Updated: 2025-05-05 10:41:10 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	const int		str_len = ft_strlen(s);
	char			*str;
	unsigned int	index;

	if (!s || !f)
		return (NULL);
	str = ft_calloc(1, str_len + 1);
	if (!str)
		return (NULL);
	index = 0;
	while (s[index])
	{
		str[index] = f(index, s[index]);
		index++;
	}
	return (str);
}
