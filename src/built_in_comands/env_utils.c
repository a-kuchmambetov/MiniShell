#include "builtins.h"

/**
 * @brief Retrieves the value of an environment variable by name.
 *
 * @param envp Environment variables array.
 * @param name Name of the variable to search for.
 * @return Pointer to value string, or NULL if not found.
 */
char *get_env_value(char **envp, const char *name)
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

/**
 * @brief Updates an existing environment variable.
 *
 * @param env Pointer to env list.
 * @param name Name of variable to update.
 * @param arg New value, or NULL to clear.
 * @return 1 if updated, 0 if not found.
 */
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
            if (arg)
                current->value = ft_strdup(arg);
            else
                current->value = NULL;
            return 1;
        }
        current = current->next;
    }
    return (0);
}

/**
 * @brief Adds a new environment variable to the list.
 *
 * @param env Pointer to env list.
 * @param key Variable name.
 * @param value Variable value (can be NULL).
 * @return 1 on success, 0 on allocation failure.
 */
int add_new_env(t_env_list *env, const char *key, const char *value)
{
    t_env_node *new_node;
    t_env_node *current;

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
 * @param data Pointer to shell data.
 * @param arg String in format "KEY" or "KEY=VALUE".
 * @return 1 on success, 0 on allocation failure.
 */
int add_or_update_env(t_shell_data *data, const char *arg)
{
    char *equal;
    char *key;
    char *value = NULL;

    equal = ft_strchr(arg, '=');
    if (equal)
    {
        key = ft_substr(arg, 0, equal - arg);
        value = ft_strdup(equal + 1);
    }
    else
        key = ft_strdup(arg);
    if (!key)
        return (0);

    if (!update_existing_env(&data->env_list, key, value))
        add_new_env(&data->env_list, key, value);
    free(key);
    free(value);
    return (1);
}

/**
 * @brief Synchronizes env_list with envp array.
 *
 * @param data Pointer to shell data.
 * @return 1 on success, 0 on allocation failure.
 */
int sync_envp(t_shell_data *data)
{
    t_env_node *current;
    int i;
    char *tmp;

    free_str_arr(data->envp);
    data->envp = malloc(sizeof(char *) * (data->env_list.len + 1));
    if (!data->envp)
        return (0);
    current = data->env_list.first;
    i = 0;
    while (current)
    {
        if (current->value)
        {
            tmp = ft_strjoin(current->key, "=");
            data->envp[i] = ft_strjoin(tmp, current->value);
            free(tmp);
        }
        else
            data->envp[i] = ft_strdup(current->key);
        current = current->next;
        i++;
    }
    data->envp[i] = NULL;
    return (1);
}