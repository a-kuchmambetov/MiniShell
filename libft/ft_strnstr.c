/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-18 09:16:53 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-18 09:16:53 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	little_len;
	size_t	index;

	if (little == NULL)
		return ((char *) big);
	if (little[0] == 0)
		return ((char *) big);
	little_len = ft_strlen(little);
	index = 0;
	while (big[index] != '\0' && index < len)
	{
		if (big[index] == *little)
			if (!ft_strncmp(&big[index], little, little_len)
				&& (index + little_len) <= len)
				return ((char *) &big[index]);
		index++;
	}
	return (NULL);
}
