/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:33 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:52:35 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	parse_input(t_shell_data *data, char *input)
{
	char			**arr;
	int				errno;
	t_token_list	*tkn_li;

	errno = 0;
	arr = split_input(input, &errno);
	if (!arr)
		return (1);
	tkn_li = create_token_list(arr, &errno);
	if (errno)
		return (free_str_arr(arr), free_token_list(tkn_li), 1);
	expand_tokens(tkn_li, data, &errno);
	if (errno)
		return (free_str_arr(arr), free_token_list(tkn_li), 1);
	split_expansion(*tkn_li, &errno);
	if (errno)
		return (free_str_arr(arr), free_token_list(tkn_li), 1);
	merge_tokens(*tkn_li, &errno);
	if (errno)
		return (free_str_arr(arr), free_token_list(tkn_li), 1);
	create_cmd_list(data, tkn_li, &errno);
	if (errno)
		return (free_str_arr(arr), free_token_list(tkn_li), 1);
	return (free_str_arr(arr), free_token_list(tkn_li), 0);
}
