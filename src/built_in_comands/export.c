#include "builtins.h"

/**
 * @brief Prints the environment variables in 'export' format.
 *
 * @param env_list The environment list structure.
 */
void print_export(t_env_list env_list)
{
    t_env_node *current;

    current = env_list.first;
    while (current)
    {
        if (current->value)
            ft_printf("declare -x %s=\"%s\"\n", current->key, current->value);
        else
            ft_printf("declare -x %s\n", current->key);
        current = current->next;
    }
}

/**
 * @brief Processes a single export argument.
 *
 * Checks if the identifier is valid and adds/updates it in the environment.
 *
 * @param data Pointer to shell data structure.
 * @param arg The argument string.
 */
int builtin_export(t_shell_data *data, char **args)
{
    int i;

    if (!args[1])
    {
        print_export(data->env_list);
        return (0);
    }
    i = 1;
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
            ft_printf("export: `%s': not a valid identifier\n", args[i]);
        else
        {
            add_or_update_env(data, args[i]);
            sync_envp(data);
        }
        i++;
    }
    return (0);
}
