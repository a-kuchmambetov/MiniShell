/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 15:45:00 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/29 15:45:00 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Prints the environment variables in 'export' format.
 * @param env_list The environment list structure.
 */
void	print_export(t_env_list env_list)
{
	t_env_node	*current;

	current = env_list.first;
	while (current)
	{
		if (current->value)
			ft_printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			ft_printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

/**
 * @brief Processes a single export argument.
 * Checks if the identifier is valid and adds/updates it in the environment.
 *
 * @param data Pointer to shell data structure.
 * @param arg The argument string.
 */
int	builtin_export(t_shell_data *data, char **args)
{
	int	i;

	if (!args[1])
	{
		print_export(data->env_list);
		return (0);
	}
	i = 1;
	// ft_printf("0 - %s\n", args[0]);
	while (args[i])
	{
		// ft_printf("%d - %s\n", i, args[i]);
		if (ft_strchr(args[i], '='))
		{
			if (process_assignment(data, args[i]))
				exit_code = 1;
		}
		else
		{
			if (handle_no_equal(args[i]))
				exit_code = 1;
		}
		process_assignment(data, args, &i);
		i++;
	}
	sync_envp(data);
	return (exit_code);
}



