/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_comand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:27:22 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 12:27:25 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	print_dash_if_needed(char **args, char *cwd)
{
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_printf("%s\n", cwd);
}

/**
 * @brief Gets current working directory, or fallback "/" if getcwd fails.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @return Duplicated string of the current working directory.
 */
char	*get_current_dir_or_fallback(t_shell_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: getcwd");
		if (data->pwd)
			cwd = ft_strdup(data->pwd);
		else
			cwd = ft_strdup("/");
	}
	return (cwd);
}

/**
 * @brief Updates the environment variable with the new PWD or OLDPWD.
 *
 * Concatenates the key and value, then updates or adds it to envp.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param key The environment variable key (e.g., "PWD=" or "OLDPWD=").
 * @param value The new value to set.
 */
void	update_env_pwd(t_shell_data *data, char *key, char *value)
{
	char	*tmp;

	tmp = ft_strjoin(key, value);
	add_or_update_env(data, tmp);
	my_free(tmp);
}

static int	cleanup_cd_error(char *oldpwd, char *path)
{
	my_free(oldpwd);
	my_free(path);
	return (1);
}

/**
 * @brief Changes directory and updates environment variables PWD and OLDPWD.
 *
 * Handles error printing and memory management for oldpwd and path.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param path Target directory path.
 * @param oldpwd Previous working directory string.
 * @param args Command arguments array.
 * @return 0 on success, 1 on failure.
 */
int	change_dir_update(t_shell_data *data, char *pa, char *old, char **args)
{
	char	*cwd;
	char	*path;

	path = pa;
	if (chdir(path) != 0)
	{
		ft_printf("cd: %s: %s\n", path, strerror(errno));
		return (cleanup_cd_error(old, path));
	}
	cwd = get_current_dir_or_fallback(data);
	if (!cwd)
		return (cleanup_cd_error(old, path));
	print_dash_if_needed(args, cwd);
	update_env_pwd(data, "OLDPWD=", old);
	update_env_pwd(data, "PWD=", cwd);
	sync_envp(data);
	if (data->pwd)
		my_free(data->pwd);
	data->pwd = ft_strdup(cwd);
	my_free(old);
	my_free(cwd);
	my_free(path);
	return (0);
}
