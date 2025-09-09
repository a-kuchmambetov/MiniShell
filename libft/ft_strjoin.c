/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-27 09:03:14 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-27 09:03:14 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	const size_t	len_s2 = ft_strlen(s2);
	const size_t	len_s1 = ft_strlen(s1);
	const size_t	len_str = len_s1 + len_s2;
	char			*str;
	size_t			size;

	if (!s1 && !s2)
		return (NULL);
	str = malloc(len_str + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len_s1 + 1);
	size = ft_strlcat(str, s2, len_str + 1);
	if (size != len_str)
		return (NULL);
	return (str);
}
