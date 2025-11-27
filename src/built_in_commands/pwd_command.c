/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:44:26 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 13:44:40 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Parses the '-L' or '-P' flag for the 'pwd' command.
 *
 * @param args Command arguments array.
 * @return Returns 1 for '-P' (physical), 0 for '-L' (logical),
 *         or -1 for invalid option.
 */
static int	parse_pwd_flag(char **args)
{
	if (!args[1])
		return (0);
	if (args[1][0] == '-' && args[1][1] && !args[1][2])
	{
		if (args[1][1] == 'P')
			return (1);
		else if (args[1][1] == 'L')
			return (0);
		ft_printf("pwd: invalid option: %s\n", args[1]);
		return (-1);
	}
	return (0);
}

/**
 * @brief Implements the 'pwd' builtin command.
 *
 * Supports '-L' (logical) and '-P' (physical) flags.
 *
 * @param data Pointer to shell data structure.
 * @param args Command arguments array.
 * @return Returns 0 on success, 1 on error or invalid option.
 */
int	builtin_pwd(t_shell_data *data, char **args)
{
	char	*pwd;
	char	*cwd;
	int		phys;

	(void)data;
	phys = parse_pwd_flag(args);
	if (phys == -1)
		return (1);
	if (!phys)
	{
		pwd = get_env_value(data->envp, "PWD");
		if (pwd && *pwd)
			return (ft_printf("%s\n", pwd), 0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	ft_printf("%s\n", cwd);
	my_free(cwd);
	return (0);
}
