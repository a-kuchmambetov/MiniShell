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
            current->value = ft_strdup(arg);
            return (1);
        }
        current = current->next;
    }
    return(0);
}

void add_new_env(t_env_list *env, const char *arg)
{
    t_env_node *new_node;
    t_env_node *current;
    char *equal;

    new_node = malloc(sizeof(t_env_node));
    equal = ft_strchr(arg, '=');
    if (!new_node)
        return;
    if (equal)
        new_node->key = ft_strndup(arg, equal - arg);
    else
        new_node->key = ft_strdup(arg);

    new_node->value = ft_strdup(arg);
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
}

int add_or_update_env(t_shell_data *data, const char *arg)
{
    char *equal;
    char *name;
    int size;

    equal = ft_strchr(arg, '=');
   if (equal)
        name = ft_strndup(arg, equal - arg);
    else
        name = ft_strdup(arg);

    if (!name)
        return;

    if (!update_existing_env(&data->env, name, arg))
        add_new_env(&data->env, arg);

    free(name);
}

void sync_envp(t_shell_data *data)
{
    t_env_node *current;
    int i;

    // Спочатку видаляємо старий масив
    free_str_arr(data->envp);

    // Виділяємо пам'ять для нового масиву + NULL в кінці
    data->envp = malloc(sizeof(char *) * (data->env.len + 1));
    if (!data->envp)
        return;

    current = data->env.first;
    i = 0;
    while (current)
    {
        data->envp[i] = ft_strdup(current->value); // "KEY=VALUE"
        current = current->next;
        i++;
    }
    data->envp[i] = NULL;
}

void print_export(t_env_list env)
{
    t_env_node *current;
    char *equal;

    current = env.first;
    while (current)
    {
        equal = ft_strchr(current->value, '=');
        if (equal)
            ft_printf("declare -x %.*s=\"%s\"\n",
                      (int)(equal - current->value),
                      current->value,
                      equal + 1);
        else
            ft_printf("declare -x %s\n", current->value);
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

// int builtin_export(t_shell_data *data, char **args)
// {
//     int i;

//     if(!args[1])
//     {
//         print_export(data->env);
//         return (0);
//     }
//     i = 1;
//     while (args[i])
//     {
//         if (!is_valid_indentifier(args[i]))
//             ft_printf(stderr,
//                 "minishell: export: `%s': not a valid identifier\n", args[i]);
//         else
//             add_or_update_env(data, args[i]);
//         i++;
//     }
//     return (0);
// }