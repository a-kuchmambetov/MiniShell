/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_comand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:26:22 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 12:26:25 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Validates cd command arguments (no more than one argument allowed).
 *
 * @param args Command arguments array.
 * @return 1 if too many arguments, 0 otherwise.
 */
static int	validate_cd_args(char **args)
{
	int count = 0;

	while (args[count])
		count++;
	if (count > 2)
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	return (0);
}


/**
 * @brief Returns the target path for the cd command.
 *
 * Handles special cases:
 * - "~" or no argument → HOME directory
 * - "-" → OLDPWD directory
 * Otherwise, returns the argument as the path.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array.
 * @return Duplicated string of the target path, or NULL on error.
 */
static char	*get_target_path(t_shell_data *data, char **args)
{
	char	*value;

	if (!args[1] || args[1][0] == '\0' || ft_strncmp(args[1], "~", 2) == 0)
	{
		value = get_env_value(data->envp, "HOME");
		if (value)
			return (ft_strdup(value));
		ft_printf("cd: HOME not set\n");
		return (NULL);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		value = get_env_value(data->envp, "OLDPWD");
		if (value)
			return (ft_strdup(value));
		ft_printf("cd: OLDPWD not set\n");
		return (NULL);
	}
	return (ft_strdup(args[1]));
}

/**
 * @brief Gets old working directory and target path for cd.
 * Allocates oldpwd using getcwd and retrieves the target path using get_target
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array.
 * @param oldpwd Pointer to store the old working directory string.
 * @return Duplicated string of the target path, or NULL on error.
 */
static char	*get_old_and_target(t_shell_data *data, char **args, char **oldpwd)
{
	char	*path;

	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
	{
		perror("getcwd");
		return (NULL);
	}
	path = get_target_path(data, args);
	if (!path)
	{
		my_free(*oldpwd);
		return (NULL);
	}
	return (path);
}

/**
 * @brief Executes the builtin cd command.
 *
 * Validates arguments, determines the target path, and performs 
 * directory change.
 * * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array.
 * @return 0 on success, 1 on failure.
 */
int	builtin_cd(t_shell_data *data, char **args)
{
	char	*oldpwd;
	char	*path;

	if (validate_cd_args(args))
		return (1);
	path = get_old_and_target(data, args, &oldpwd);
	if (!path)
		return (1);
	return (change_dir_update(data, path, oldpwd, args));
}
