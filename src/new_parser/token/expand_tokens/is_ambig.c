/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_ambig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:06 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:52:07 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_tokens.h"

int	is_ambig(char *src)
{
	int	i;
	int	words;
	int	in_word;

	if (!src)
		return (0);
	i = 0;
	words = 0;
	in_word = 0;
	while (src[i])
	{
		if (src[i] == ' ' || src[i] == '\t' || src[i] == '\n')
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			words++;
			if (words > 1)
				return (1);
		}
		i++;
	}
	return (0);
}
