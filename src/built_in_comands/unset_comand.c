#include "builtins.h"

/**
 * @brief Removes an environment variable node by key from the linked list.
 *
 * Frees the memory of the removed node and updates list pointers.
 *
 * @param env Pointer to the environment list.
 * @param key Key of the variable to remove.
 */
static void remove_env_node(t_env_list *env, const char *key)
{
	t_env_node *current;
	t_env_node *prev;

	current = env->first;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				env->first = current->next;
			free(current->key);
			free(current->value);
			free(current);
			env->len--;
			return;
		}
		prev = current;
		current = current->next;
	}
}


/**
 * @brief Implements the 'unset' builtin command.
 *
 * Removes environment variables by key if identifiers are valid.
 *
 * @param data Pointer to shell data structure.
 * @param args Command arguments array.
 * @return Always returns 0.
 */
int builtin_unset(t_shell_data *data, char **args)
{
	int i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
			ft_putstr_fd("minishell: unset: not a valid identifier\n", 2);
		else
			remove_env_node(&data->env_list, args[i]);
		i++;
	}
	sync_envp(data);
	return (0);
}
