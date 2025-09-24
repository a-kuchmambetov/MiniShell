#include "builtins.h"

int is_valid_identifier(const char *str)
{
    int i;

    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    i = 0;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
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
    return 1;
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

void print_export(t_env_list env)
{
    t_env_node *current = env.first;

    while (current)
    {
        if (current->value)
            ft_printf("declare -x %s=\"%s\"\n", current->key, current->value);
        else
            ft_printf("declare -x %s\n", current->key);
        current = current->next;
    }
}

int builtin_export(t_shell_data *data, char **args)
{
    int i;

    if (!args[1])
    {
        print_export(data->env); // <-- використовуємо список, а не envp
        return (0);
    }

    i = 1;
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
            ft_printf("minishell: export: `%s': not a valid identifier\n", args[i]);
        else
        {
            add_or_update_env(data, args[i]);
            sync_envp(data);
        }
        i++;
    }
    return (0);
}
