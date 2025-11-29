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
 * @brief Processes a KEY=VALUE assignment for the `export` builtin.
 *
 * Validates the identifier, extracts the key and value, constructs the
 * final KEY=VALUE string, and updates or adds the variable to the env list.
 *
 * @param data Pointer to shell data structure.
 * @param arg  Argument containing KEY=VALUE.
 * @return 0 on success, 1 on error (invalid identifier or allocation failure).
 */
static int	process_assignment(t_shell_data *data, char *arg)
{
	char	*name;
	char	*value;
	char	*final;
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (1);
	name = ft_substr(arg, 0, eq - arg);
	if (!name)
		return (1);
	if (!is_valid_identifier(name))
		return (error_not_valid_identifier(arg, name));
	value = ft_strdup(eq + 1);
	if (!value)
		return (my_free(name), 1);
	final = build_final_pair(name, value);
	if (!final)
		return (my_free(name), my_free(value), 1);
	if (!add_or_update_env(data, final))
		return (clean_process_assigment(name, value, final), 1);
	clean_process_assigment(name, value, final);
	return (0);
}

/**
 * @brief Processes `export NAME` (identifier without '=' assignment).
 *
 * Validates the identifier and ensures it exists in the environment list.
 * If it does not exist, adds it with a NULL value.
 *
 * @param arg  Variable name.
 * @param data Shell data structure.
 * @return 0 on success, 1 on invalid identifier or allocation failure.
 */
static int	handle_no_equal(char *arg, t_shell_data *data)
{
	t_env_node	*node;

	if (!is_valid_identifier(arg))
	{
		ft_print_err("export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	node = data->env_list.first;
	while (node)
	{
		if (ft_strncmp(node->key, arg, ft_strlen(arg) + 1) == 0)
			return (0);
		node = node->next;
	}
	node = malloc(sizeof(t_env_node));
	if (!node)
		return (1);
	node->key = ft_strdup(arg);
	node->value = NULL;
	node->next = data->env_list.first;
	data->env_list.first = node;
	data->env_list.len++;
	return (0);
}

/**
 * @brief Implements the `export` builtin.
 *
 * Behavior:
 * - No arguments → prints export list in sorted format.
 * - For each ARG:
 *      - KEY=VALUE → validate and assign/update variable.
 *      - KEY       → validate and ensure variable exists with NULL value.
 * - Updates data->envp after processing.
 *
 * @param data Pointer to shell data.
 * @param args Command arguments.
 * @return 0 if all assignments were valid, 1 if any identifier was invalid.
 */
int	builtin_export(t_shell_data *data, char **args)
{
	int	i;
	int	exit_code;

	exit_code = 0;
	if (!args[1] || args[1][0] == '\0')
		return (print_export_list(data->env_list.first), 0);
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			if (process_assignment(data, args[i]))
				exit_code = 1;
		}
		else
		{
			if (handle_no_equal(args[i], data))
				exit_code = 1;
		}
		i++;
	}
	sync_envp(data);
	return (exit_code);
}
