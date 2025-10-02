#include "builtins.h"

char	*get_env_value(char **envp, const char *name)
{
	int i = 0;
	int len = ft_strlen(name);

	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int update_existing_env(t_env_list *env, const char *name, const char *arg)
{
    t_env_node *current;
    size_t len;

    current = env->first;
    len = ft_strlen(name);
    while (current)
    {
        if (!ft_strncmp(current->key, name, len))
        {
            free(current->value);
            current->value = arg ? ft_strdup(arg) : NULL;
            return (1);
        }
        current = current->next;
    }
    return(0);
}

int add_new_env(t_env_list *env, const char *key, const char *value)
{
    t_env_node *new_node;
    t_env_node *current;

    new_node = malloc(sizeof(t_env_node));
    if (!new_node)
        return(0);
    new_node->key = ft_strdup(key);
    new_node->value = value ? ft_strdup(value) : NULL;
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
    return(1);
}

int add_or_update_env(t_shell_data *data, const char *arg)
{
    char *equal = ft_strchr(arg, '=');
    char *key;
    char *value = NULL;

    if (equal)
    {
        key = ft_substr(arg, 0, equal - arg);
        value = ft_strdup(equal + 1);
    }
    else
        key = ft_strdup(arg);
    if (!key) 
        return (0);

    if (!update_existing_env(&data->env, key, value))
        add_new_env(&data->env, key, value);
    free(key);
    free(value);
    return (1);
}

int sync_envp(t_shell_data *data)
{
    t_env_node *current;
    int i;
    char *tmp;
    free_str_arr(data->envp); 
    data->envp = malloc(sizeof(char *) * (data->env.len + 1));
    if (!data->envp)
        return(0);
    current = data->env.first;
    i = 0;
    while (current)
    {
        if (current->value)
        {
            tmp = ft_strjoin(current->key, "=");            // key + "="
            data->envp[i] = ft_strjoin(tmp, current->value); // (key + "=") + value
            free(tmp);
        }
        else
        {
            data->envp[i] = ft_strdup(current->key);
        }
        current = current->next;
        i++;
    }
    data->envp[i] = NULL;
    return(1);
}