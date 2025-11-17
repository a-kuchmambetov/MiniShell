/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_comands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:13:07 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 12:18:53 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Checks if a given command is a builtin command.
 *
 * Supported builtin commands:
 * echo, cd, pwd, export, рunset, env, exit
 *norm bui  
 * @param cmd Command string to check.
 * @return 1 if cmd is a builtin, 0 otherwise.
 */
int	is_builtin(char *cmd)
{
	if (!cmd || !cmd[0])
		return (0);
	if (!ft_strncmp(cmd, "echo", 5))
		return (1);
	if (!ft_strncmp(cmd, "cd", 3))
		return (1);
	if (!ft_strncmp(cmd, "pwd", 4))
		return (1);
	if (!ft_strncmp(cmd, "export", 7))
		return (1);
	if (!ft_strncmp(cmd, "unset", 6))
		return (1);
	if (!ft_strncmp(cmd, "env", 4))
		return (1);
	if (!ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

/**
 * @brief Executes   the appropriate builtin command based on args[0].
 *
 * Maps command strings to their corresponding builtin functions.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array; args[0] is the command name.
 * @return The return value of the executed builtin, or 1 if not recognized.
 */
int	exec_builtin(t_shell_data *data, char **args)
{
	if (!ft_strncmp(args[0], "echo", 5))
		return (builtin_echo(args));
	else if (!ft_strncmp(args[0], "cd", 3))
		return (builtin_cd(data, args));
	else if (!ft_strncmp(args[0], "pwd", 4))
		return (builtin_pwd(data, args));
	else if (!ft_strncmp(args[0], "export", 7))
		return (builtin_export(data, args));
	else if (!ft_strncmp(args[0], "unset", 6))
		return (builtin_unset(data, args));
	else if (!ft_strncmp(args[0], "env", 4))
		return (builtin_env(data));
	else if (!ft_strncmp(args[0], "exit", 5))
		return (builtin_exit(data, args));
	return (1);
}
