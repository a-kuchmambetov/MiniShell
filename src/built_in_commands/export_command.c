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


/* Frees temporary strings after processing */
static void	clean_process_assigment(char *name, char *val, char *final)
{
	my_free(name);
	my_free(val);
	my_free(final);
}

/* Error message for invalid identifier */
static int	error_not_valid_identifier(char *arg, char *name)
{
	ft_print_err("export: `%s' not a valid identifier\n", arg);
	my_free(name);
	return (1);
}


/* Process KEY=VALUE assignments */
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
		return(error_not_valid_identifier(arg, name));
	value = ft_strdup(eq + 1);
	if (!value)
		return (my_free(name),1);
	final = build_final_pair(name, value);
	if (!final)
		return (my_free(name), my_free(value),1);
	if (!add_or_update_env(data, final))
		return(clean_process_assigment(name, value, final), 1);
	clean_process_assigment(name, value, final);
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
/* Handle export NAME without '=' */
static int	handle_no_equal(char *arg, t_shell_data *data)
{
	t_env_node	*node;

	if (!is_valid_identifier(arg))
	{
		ft_print_err("export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	// Шукаємо, чи така змінна вже існує
	node = data->env_list.first;
	while (node)
	{
		if (ft_strncmp(node->key, arg, ft_strlen(arg) + 1) == 0)
			return (0); // вже є, нічого не робимо
		node = node->next;
	}
	node = malloc(sizeof(t_env_node));
	if (!node)
		return (1); // помилка виділення пам'яті
	node->key = ft_strdup(arg);
	node->value = NULL; // без значення
	node->next = data->env_list.first;
	data->env_list.first = node;
	data->env_list.len++;
	return (0);
}


/**
 * @brief Builtin: export command.
 */
int	builtin_export(t_shell_data *data, char **args)
{
	int		i;
	int		exit_code;

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
