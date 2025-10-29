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
 * @brief Processes a single "KEY=VALUE" assignment for export.
 *
 * Validates the key name, collects and cleans the value, and
 * adds or updates it in the environment list.
 *
 * @param data Pointer to the shell data.
 * @param args Command arguments array.
 * @param i Pointer to current argument index.
 * @return 0 on success, 1 if invalid identifier.
 */
static int	process_assignment(t_shell_data *data, char **args, int *i)
{
	char	*name;
	char	*val;
	char	*clean;
	char	*final;
	char	*eq;

	eq = ft_strchr(args[*i], '=');
	if (!eq)
		return (0);
	name = ft_substr(args[*i], 0, eq - args[*i]);
	if (!is_valid_identifier(name))
	{
		ft_print_err("export: not a valid identifier\n");
		free(name);
		return (1);
	}
	val = collect_value_after_equal(args, i);
	clean = strip_outer_quotes(val);
	final = build_final_pair(name, clean);
	add_or_update_env(data, final);
	free(name);
	free(val);
	free(clean);
	free(final);
	return (0);
}

/**
 * @brief Handles the 'export' builtin command.
 * Iterates over arguments, validates each variable name, and
 * updates or adds the variable to the environment list.
 * @param data Pointer to the shell data.
 * @param args Command arguments (e.g., export VAR=value).
 * @return Always 0 (success).
 */
int	builtin_export(t_shell_data *data, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!ft_strchr(args[i], '='))
		{
			ft_printf("export: ignoring '%s' (no assignment)\n", args[i]);
			i++;
			continue ;
		}
		process_assignment(data, args, &i);
		i++;
	}
	sync_envp(data);
	return (0);
}


