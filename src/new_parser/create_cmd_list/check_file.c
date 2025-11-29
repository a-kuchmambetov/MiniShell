/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:49:17 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:49:19 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "create_cmd_list_utils.h"
#include <errno.h>

// Check with chatGPT for edge cases regarding filenames
int	check_file_open(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (*filename == '\0' || *filename == '/')
	{
		ft_print_err("syntax error near unexpected token `%s'\n", filename);
		return (close(fd), 1);
	}
	access(filename, R_OK);
	if (errno == ENOENT)
	{
		ft_print_err("%s: No such file or directory\n", filename);
		return (close(fd), 1);
	}
	else if (errno == EACCES)
	{
		ft_print_err("%s: Permission denied\n", filename);
		return (close(fd), 1);
	}
	close(fd);
	return (0);
}

int	check_file_create(const char *filename, const int redir_type)
{
	int	fd;

	if (*filename == '\0' || *filename == '/')
	{
		ft_print_err("syntax error near unexpected token `%s'\n", filename);
		return (1);
	}
	if (redir_type == REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	access(filename, R_OK);
	if (errno == ENOENT)
	{
		ft_print_err("%s: No such file or directory\n", filename);
		return (close(fd), 1);
	}
	else if (errno == EACCES)
	{
		ft_print_err("%s: Permission denied\n", filename);
		return (close(fd), 1);
	}
	close(fd);
	return (0);
}
