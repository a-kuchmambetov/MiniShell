/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-15 12:01:46 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-15 12:01:46 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dest_cp;
	unsigned char	*src_cp;

	dest_cp = (unsigned char *) dest;
	src_cp = (unsigned char *) src;
	if (!dest && !src)
		return (0);
	if (dest <= src)
		ft_memcpy(dest_cp, src_cp, n);
	else if (dest > src)
	{
		dest_cp += n - 1;
		src_cp += n - 1;
		while (n--)
			*dest_cp-- = *src_cp--;
	}
	return (dest);
}
