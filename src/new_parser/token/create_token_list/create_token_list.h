/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_list.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:50:59 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 13:12:10 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CREATE_TOKEN_LIST_H
# define CREATE_TOKEN_LIST_H

# include "../../parser.h"

void            free_token_list(t_token_list *tkn_li);
void            free_token_node(t_token_node *node);

t_token_list    *create_token_list(char **arr, int *errno);
t_token_node    *set_tkn(char **arr, int *i, int *errno);

#endif