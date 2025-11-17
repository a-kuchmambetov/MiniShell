/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:21:50 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 13:21:59 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Implements the 'env' builtin command.
 *
 * Prints all environment variables that contain '='.
 * If any arguments are passed, prints an error message.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array.
 * @return 0 on success, 1 if extra arguments are provided.
 */
int	builtin_env(t_shell_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strchr(data->envp[i], '='))
			ft_putendl_fd(data->envp[i], 1);
		i++;
	}
	return (0);
}
