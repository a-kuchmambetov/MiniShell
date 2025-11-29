/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:49:34 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 13:10:42 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLIT_INPUT_H
# define SPLIT_INPUT_H

# include "../parser.h"

char	**split_input(const char *s, int *errno);

int		is_delim(const char *str);
void	skip_spaces(const char *s, t_split_data *dt);
int		handle_add_row(const char *s, t_split_data *dt);
int		add_row(t_split_data *dt, char *s);
int		process_delimiter(const char *s, t_split_data *dt, int delim_len);

#endif