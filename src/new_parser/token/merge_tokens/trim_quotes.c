/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:21 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 13:00:30 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "merge_tokens.h"

char	*trim_quotes(char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	if ((str[0] == '\"' || str[0] == '\'') && str[0] == str[ft_strlen(str) - 1])
	{
		while (str[i])
		{
			str[i] = str[i + 1];
			i++;
		}
		str[i - 1] = '\0';
		str[i - 2] = '\0';
	}
	return (str);
}

char	*trim_space_before(char *str)
{
	int	i;
	int	j;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] == ' ')
		i++;
	if (i == 0)
		return (str);
	j = 0;
	while (str[i])
	{
		str[j] = str[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

char	*trim_space_after(char *str)
{
	const int	str_len = ft_strlen(str) - 1;
	int			i;
	int			j;

	if (!str)
		return (NULL);
	i = str_len;
	while (str[i] == ' ')
		i--;
	if (i == str_len)
		return (str);
	j = 0;
	while (str[i])
	{
		str[j] = str[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

char	*trim_space_both(char *str)
{
	char	*res;

	res = trim_space_before(str);
	res = trim_space_after(res);
	return (res);
}
