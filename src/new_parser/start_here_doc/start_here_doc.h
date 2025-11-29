/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_here_doc.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:50:49 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 13:16:04 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef START_HERE_DOC_H
# define START_HERE_DOC_H

# include "../parser.h"

int		check_signal_hook(void);
void	handle_sigint_heredoc(int sig);

int		check_file_name(char **name);
int		create_file(char **filename, int *fd);

void	print_error(const char *eof_word);
int		compare_eof(const char *input, const char *eof_word);

#endif