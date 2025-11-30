/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-16 10:10:47 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-16 10:10:47 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	index;
	size_t	dst_lenght;

	dst_lenght = ft_strlen(dst);
	index = 0;
	if (size <= 0)
		return (ft_strlen(src));
	while ((dst_lenght + index < size - 1) && src[index] != '\0')
	{
		dst[dst_lenght + index] = src[index];
		index++;
	}
	dst[dst_lenght + index] = '\0';
	if (dst_lenght >= size)
	{
		dst_lenght = size;
	}
	return (dst_lenght + ft_strlen(src));
}
