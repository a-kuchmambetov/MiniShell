/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:18 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:52:19 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MERGE_TOKENS_H
# define MERGE_TOKENS_H

# include "../../parser.h"

void	merge_tokens(t_token_list tkn_li, int *errno);

char	*trim_quotes(char *str);
char	*trim_space_before(char *str);
char	*trim_space_after(char *str);
char	*trim_space_both(char *str);

#endif