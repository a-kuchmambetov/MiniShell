/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-27 10:45:43 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-27 10:45:43 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	charcmp(const char s1, const char *set)
{
	if (!set)
		return (0);
	while (*set)
		if (*set++ == s1)
			return (1);
	return (0);
}

static char	*trim(const char *str, char const *set, size_t len)
{
	size_t	index;
	size_t	trim_left;
	size_t	trim_right;
	char	*res;

	trim_left = 0;
	trim_right = 0;
	index = 0;
	while (index < len && charcmp(str[index], set))
		index++;
	trim_left = index;
	if (index >= len)
		return (ft_calloc(sizeof(char), 1));
	index = 0;
	while (len - index > 0 && charcmp(str[len - index - 1], set))
		index++;
	trim_right = len - index;
	res = ft_calloc(sizeof(char), trim_right - trim_left + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, str + trim_left, trim_right - trim_left + 1);
	return (res);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;

	if (!s1)
		return (NULL);
	str = trim(s1, set, ft_strlen(s1));
	return (str);
}
