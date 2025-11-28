/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:45:00 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/29 19:45:00 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Prints the environment in declare -x format.
 */
void	print_export_list(t_env_node *node)
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

/* Build "KEY=VALUE" string */
char	*build_final_pair(char *name, char *clean)
{
	char	*pair;
	char	*final;

	pair = ft_strjoin(name, "=");
	if (!pair)
		return (NULL);
	final = ft_strjoin(pair, clean);
	my_free(pair);
	return (final);
}

/* Frees temporary strings after processing */
void	clean_process_assigment(char *name, char *val, char *final)
{
	my_free(name);
	my_free(val);
	my_free(final);
}

/* Error message for invalid identifier */
int	error_not_valid_identifier(char *arg, char *name)
{
	ft_print_err("export: not a valid identifier\n", arg);
	my_free(name);
	return (1);
}
