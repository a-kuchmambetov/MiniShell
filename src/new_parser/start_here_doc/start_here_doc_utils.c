/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_here_doc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:50:41 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:59:30 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "start_here_doc.h"

int	check_signal_hook(void)
{
	if (g_signal_received)
	{
		rl_done = 1;
		return (0);
	}
	return (0);
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(STDOUT_FILENO, "\n", 1);
}

void	print_error(const char *eof_word)
{
	ft_print_err("warning:");
	ft_putstr_fd(COLOR_RED, 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd((char *)eof_word, 2);
	ft_putstr_fd("')\n", 2);
	ft_putstr_fd(COLOR_RESET, 2);
}

int	compare_eof(const char *input, const char *eof_word)
{
	char	*trimmed_eof;
	int		result;

	if (!input || !eof_word)
		return (0);
	trimmed_eof = ft_strtrim((char *)eof_word, "\'\"");
	if (!trimmed_eof)
		return (0);
	result = ft_strncmp(input, trimmed_eof, ft_strlen(input)) == 0;
	my_free(trimmed_eof);
	return (result);
}
