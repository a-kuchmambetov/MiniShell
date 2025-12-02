/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:39:10 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 13:10:01 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>

static void	exit_exec_failure(char **argv, char *path, t_shell_data *data)
{
	int	code;

	if (errno == EACCES)
		code = 126;
	else
		code = 127;
	perror(argv[0]);
	if (path)
		free(path);
	my_exit(code, data);
}

static char	*find_executable(t_shell_data *data, char *command)
{
	int		i;
	char	*full_path;

	i = 0;
	if (!data->paths)
		return (NULL);
	while (data->paths[i])
	{
		full_path = malloc(ft_strlen(data->paths[i]) + ft_strlen(command) + 2);
		if (!full_path)
			return (NULL);
		ft_strlcpy(full_path, data->paths[i], ft_strlen(data->paths[i]) + 1);
		ft_strlcat(full_path, "/", ft_strlen(full_path) + 2);
		ft_strlcat(full_path, command, ft_strlen(full_path) + ft_strlen(command)
			+ 1);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	exec_external(t_shell_data *data, char **argv)
{
	char	*path;

	if (!argv || !argv[0])
		my_exit(0, data);
	if (ft_strchr(argv[0], '/'))
	{
		execve(argv[0], argv, data->envp);
		exit_exec_failure(argv, NULL, data);
	}
	path = find_executable(data, argv[0]);
	if (!path)
	{
		ft_print_err("%s: command not found\n", argv[0]);
		my_exit(127, data);
	}
	execve(path, argv, data->envp);
	exit_exec_failure(argv, path, data);
}
