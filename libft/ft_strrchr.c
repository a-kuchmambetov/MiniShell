/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-16 10:33:15 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-16 10:33:15 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		index;
	char	*ptr;

	index = 0;
	ptr = NULL;
	while (s[index])
	{
		if (s[index] == (char) c)
			ptr = (char *) &s[index];
		index++;
	}
	if (s[index] == (char) c)
		ptr = (char *) &s[index];
	return (ptr);
}
