#include "builtins.h"

static void remove_env_node(t_env_list *env, const char *key)
{
	t_env_node *current = env->first;
	t_env_node *prev = NULL;

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

int	builtin_unset(t_shell_data *data, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
			ft_putstr_fd("minishell: unset: not a valid identifier\n", 2);
		else
			remove_env_node(&data->env, args[i]);
		i++;
	}
    sync_envp(data);
	return (0);
}
