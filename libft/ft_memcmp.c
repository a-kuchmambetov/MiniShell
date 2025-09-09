/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-18 08:56:46 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-18 08:56:46 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s1_ptr;
	unsigned char	*s2_ptr;
	size_t			index;

	s1_ptr = (unsigned char *) s1;
	s2_ptr = (unsigned char *) s2;
	index = 0;
	while (index < n)
	{
		if (s1_ptr[index] != s2_ptr[index])
			return ((unsigned char) s1_ptr[index]
				- (unsigned char) s2_ptr[index]);
		index++;
	}
	return (0);
}
