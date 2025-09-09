/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-18 08:28:26 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-18 08:28:26 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	comp = (unsigned char) c;
	unsigned char		*ptr;
	size_t				index;

	ptr = (unsigned char *) s;
	index = 0;
	while (index < n)
	{
		if (ptr[index] == comp)
			return ((void *) ptr + index);
		index++;
	}
	return (NULL);
}
