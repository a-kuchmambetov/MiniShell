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
 * @brief Adds or updates an environment variable from assignment.
 */

static int	process_assignment(t_shell_data *data, char **args, int *i)
{
	char	*name;
	char	*val;
	char	*final;
	char	*eq;

	eq = ft_strchr(args[*i], '=');
	if (!eq)
		return (1); // некоректний синтаксис

	name = ft_substr(args[*i], 0, eq - args[*i]);
	if (!name)
		return (1); // malloc fail
	if (!is_valid_identifier(name))
	{
		ft_print_err("export: `%s' not a valid identifier\n", args[1]);
		return (free(name),1);
	}
	val = collect_value_after_equal(args, i);
	if (!val)
		return (free(name),1);
	final = build_final_pair(name, val);
	if (!final)
	{
		free(name);
		free(val);
		return (1);
	}
	if (!add_or_update_env(data, final))
	{
		free(name);
		free(val);
		free(final);
		return (1);
	}
	free(name);
	free(val);
	free(final);
	return (0);
}


/**
 * @brief Prints the environment in declare -x format.
 */
static void	print_export_list(t_env_node *node)
{
	while (node)
	{
		if (node->value)
			ft_printf("declare -x %s=\"%s\"\n", node->key, node->value);
		else
			ft_printf("declare -x %s\n", node->key);
		node = node->next;
	}
}

/**
 * @brief Builtin: export command.
 *
 * Supports:
 * - export               → prints all variables
 * - export VAR           → adds VAR if valid
 * - export VAR=VALUE     → adds or updates with value (supports quotes/spaces)
 */
int	builtin_export(t_shell_data *data, char **args)
{
	int		i;
	char	*trimmed;
	int		exit_code;
	int		res;

	exit_code = 0;
	if (!args[1] || args[1][0] == '\0')
		return (print_export_list(data->env_list.first), 0);

	i = 1;
	while (args[i])
	{
		// 🔹 копіюємо або трімимо, щоб не втратити пробіли в значеннях
		if (ft_strchr(args[i], '='))
			trimmed = ft_strdup(args[i]);
		else
			trimmed = ft_strtrim(args[i], " \t");
		if (!trimmed)
			return (1); // malloc error
		// 🔹 якщо порожній аргумент — пропускаємо
		if (trimmed[0] == '\0')
		{
			my_free(trimmed);
			i++;
			continue;
		}
			
		// 🔹 без '=' → просто ім'я
		if (!ft_strchr(trimmed, '='))
		{
			if (!is_valid_identifier(trimmed))
			{
				ft_print_err("export: `%s': not a valid identifier\n", trimmed);
				exit_code = 1;
			}
			else
			{
				if (!add_or_update_env(data, trimmed))
				{
					my_free(trimmed);
					return (1);
				}
			}
		}
		// 🔹 з '=' → повне присвоєння
		else
		{
			res = process_assignment(data, args, &i);
			if (res == 1)
				exit_code = 1;
		}
		my_free(trimmed);
		i++;
	}
	sync_envp(data);
	return (exit_code);
}



