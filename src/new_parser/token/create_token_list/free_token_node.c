/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:51:03 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:57:51 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "create_token_list.h"

void	free_token_node(t_token_node *node)
{
	my_free(node->value);
	my_free(node);
}
