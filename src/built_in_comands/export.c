#include "builtins.h"


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
