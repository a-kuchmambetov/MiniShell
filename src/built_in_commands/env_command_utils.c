/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_command_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:22:51 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 13:22:53 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Constructs a "key=value" string for use in envp.
 *
 * Allocates and returns a new string in the format:
 *     key + "=" + value
 *
 * @param key   Name of the environment variable.
 * @param value Value of the environment variable.
 * @return Newly allocated "key=value" string, or NULL on failure.
 */
static char	*make_env_entry(const char *key, const char *value)
{
	char	*tmp;
	char	*res;

	if (ft_strncmp(key, "?", 2) == 0)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, value);
	my_free(tmp);
	if (!res)
		return (NULL);
	return (res);
}

/**
 * @brief Converts an environment linked list into an envp-style array.
 *
 * Each node becomes:
 *   - "key=value" if value exists
 *   - "key"       if value is NULL
 *
 * The resulting array is NULL-terminated and must be freed by the caller.
 *
 * @param node Pointer to the first environment node.
 * @param envp Already allocated envp array to fill.
 * @return 1 on success, 0 if any allocation fails.
 */
static int	fill_envp(t_env_node *node, char **envp)
{
	int	i;

	i = 0;
	while (node)
	{
		if (node->value)
		{
			envp[i] = make_env_entry(node->key, node->value);
			if (!envp[i])
				return (0);
		}
		else
		{
			envp[i] = ft_strdup(node->key);
			if (!envp[i])
				return (0);
		}
		i++;
		node = node->next;
	}
	envp[i] = NULL;
	return (1);
}

/**
 * @brief Synchronizes env_list with envp array.
 *
 * Frees the previous envp array and rebuilds it from the linked list
 * stored in data->env_list.
 *
 * @param data Shell data containing env_list and envp.
 * @return 1 on success, 0 on allocation failure.
 */
int	sync_envp(t_shell_data *data)
{
	free_str_arr(data->envp);
	data->envp = malloc(sizeof(char *) * (data->env_list.len + 1));
	if (!data->envp)
		return (0);
	return (fill_envp(data->env_list.first, data->envp));
}
