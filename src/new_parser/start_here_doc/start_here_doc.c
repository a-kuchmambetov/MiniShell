/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_here_doc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:50:45 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:50:46 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "start_here_doc.h"

int	delete_here_doc(const char *filename)
{
	if (!filename)
	{
		ft_print_err("error: invalid filename of here-document\n");
		return (1);
	}
	if (unlink(filename) < 0)
	{
		ft_print_err("error: unable to delete here-document temp file\n");
		return (1);
	}
	filename = NULL;
	return (0);
}

static void	cleanup_here_doc(char *input, int fd, char **filename)
{
	close(fd);
	if (input)
	{
		free(input);
		input = NULL;
	}
	if (filename && *filename)
	{
		delete_here_doc(*filename);
		free(*filename);
		*filename = NULL;
	}
}

static char	*start_expansion(t_env_list env, char *input, int *errno)
{
	char	*res;
	int		type;

	type = TOKEN_TEXT;
	res = process_expansion(env, input, errno, &type);
	if (*errno)
		return (NULL);
	my_free(input);
	return (res);
}

static int	write_here_doc(t_env_list env, const char *eof_word,
		t_here_doc_data h_d_data, int type)
{
	char	*input;
	int		errno;

	errno = 0;
	while (1)
	{
		input = readline("> ");
		if (g_signal_received)
			return (cleanup_here_doc(input, h_d_data.fd, h_d_data.filename), 130);
		if (!input)
			return (close(h_d_data.fd), print_error(eof_word), 0);
		if (compare_eof(input, eof_word))
			break ;
		if (type == TOKEN_HEREDOC)
			input = start_expansion(env, input, &errno);
		if (errno)
			return (cleanup_here_doc(input, h_d_data.fd, h_d_data.filename), 1);
		ft_putendl_fd(input, h_d_data.fd);
		free(input);
	}
	if (input)
		free(input);
	close(h_d_data.fd);
	return (0);
}

int	start_here_doc(t_env_list env, const char *eof_word, char **filename, int type)
{
	int		result;
	void	(*old_handler)(int);
	t_here_doc_data h_d_data;

	h_d_data.fd = -1;
	h_d_data.filename = filename;
	if (!h_d_data.filename)
		return (1);
	if (*h_d_data.filename)
	{
		my_free(*h_d_data.filename);
		*h_d_data.filename = NULL;
	}
	g_signal_received = 0;
	rl_done = 0;
	rl_event_hook = check_signal_hook;
	old_handler = signal(SIGINT, handle_sigint_heredoc);
	if (create_file(h_d_data.filename, &h_d_data.fd))
		return (signal(SIGINT, old_handler), 1);
	result = write_here_doc(env, eof_word, h_d_data, type);
	g_signal_received = 0;
	rl_done = 0;
	rl_event_hook = NULL;
	signal(SIGINT, old_handler);
	return (result);
}
