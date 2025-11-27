/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:13:01 by unadoroz          #+#    #+#             */
/*   Updated: 2025/11/27 15:13:13 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Updates the value of an existing environment variable.
 *
 * Searches the env list for a variable whose key matches @p name.
 * If found, replaces its value with @p arg (or NULL if @p arg is NULL).
 *
 * @param env  Pointer to the environment list.
 * @param name Name of the variable to update.
 * @param arg  New value, or NULL to unset the variable's value.
 * @return 1 if the variable was found and updated, 0 otherwise.
 */
int	update_existing_env(t_env_list *env, const char *name, const char *arg)
{
	t_env_node	*current;
	size_t		len;

	current = env->first;
	len = ft_strlen(name);
	while (current)
	{
		if (!ft_strncmp(current->key, name, len) && current->key[len] == '\0')
		{
			free(current->value);
			if (arg)
				current->value = ft_strdup(arg);
			else
				current->value = NULL;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/**
 * @brief Adds a new environment variable to the end of the list.
 *
 * Allocates a new node and appends it to @p env. The value may be NULL,
 * representing a variable without an assigned value.
 *
 * @param env   Pointer to the environment list.
 * @param key   Variable name (must not be NULL).
 * @param value Variable value (may be NULL).
 * @return 1 on success, 0 if allocation fails.
 */
int	add_new_env(t_env_list *env, const char *key, const char *value)
{
	t_env_node	*new_node;
	t_env_node	*current;

	new_node = malloc(sizeof(t_env_node));
	if (!new_node)
		return (0);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!env->first)
		env->first = new_node;
	else
	{
		current = env->first;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	env->len++;
	return (1);
}

/**
 * @brief Adds or updates an environment variable from a "KEY=VALUE" string.
 *
 * Parses @p arg into key and value. If the variable already exists,
 * its value is updated. Otherwise, a new variable is added to the list.
 *
 * @param data Shell state containing the environment list.
 * @param arg  Input string in the form "KEY=VALUE" or just "KEY".
 * @return 1 on success, 0 on allocation failure.
 */
int	add_or_update_env(t_shell_data *data, const char *arg)
{
	char	*equal;
	char	*key;
	char	*value;
	int		res;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key = ft_substr(arg, 0, equal - arg);
		value = ft_strdup(equal + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	if (!key || (equal && !value))
		return (cleanup_return(key, value, 0));
	res = update_existing_env(&data->env_list, key, value);
	if (!res)
		res = add_new_env(&data->env_list, key, value);
	return (cleanup_return(key, value, res));
}
