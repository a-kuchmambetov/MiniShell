/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-23 09:54:19 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-23 09:54:19 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	const int	size = ft_strlen(s) + 1;
	char		*str;

	str = ft_calloc(1, size);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s, size);
	return (str);
}
