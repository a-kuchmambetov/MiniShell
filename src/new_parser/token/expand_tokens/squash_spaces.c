/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   squash_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:11 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:54:30 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_tokens.h"

char	*squash_spaces(char *src)
{
	int	i;
	int	j;
	int	in_space;

	i = -1;
	j = 0;
	in_space = 0;
	while (src[++i])
	{
		if (src[i] != ' ')
		{
			src[j++] = src[i];
			in_space = 0;
		}
		else if (!in_space)
		{
			src[j++] = ' ';
			in_space = 1;
		}
	}
	src[j] = '\0';
	return (src);
}
