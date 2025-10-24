#include "process_expansion_utils.h"

static int find_key_len(const char *str)
{
    int len;

    len = 0;
    if (!str || !*str)
        return (0);
    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
    return (len);
}

static char *get_value_by_key(t_env_list env, const char *name)
{
    t_env_node *current;
    size_t len;

    current = env.first;
    if (!name || !*name)
        return (NULL);
    len = ft_strlen(name);
    while (current)
    {
        if (!ft_strncmp(current->key, name, len))
        {
            return (current->value);
        }
        current = current->next;
    }
    return (NULL);
}

int get_value_from_str(t_env_list env, t_split_data *dt, const char *input, char **value)
{
    const int key_len = find_key_len(input + dt->i + 1);
    char *key;
    char *env_value;

    if (!input)
        return (1);
    if (key_len < 1)
    {
        *value = ft_calloc(1, 1);
        if (!*value)
            return (1);
        return (0);
    }
    key = ft_strndup(input + dt->i + 1, key_len);
    if (!key)
        return (1);
    env_value = get_value_by_key(env, key);
    if (env_value)
        *value = ft_strdup(env_value);
    else
        *value = ft_calloc(1, 1);
    if (!*value)
        return (free(key), 1);
    dt->i += key_len;
    return (free(key), 0);
}