#include "../main.h"

void free_env_list(t_env_list *env)
{
    t_env_node *current;
    t_env_node *next;

    if (!env)
        return ;
    current = env->first;
    while (current)
    {
        if (!current)
            break ;
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}

void free_str_arr(char **str_arr)
{
    int i;

    i = 0;
    if (!str_arr)
        return ;
    while (str_arr[i])
    {
        if (str_arr[i])
            free(str_arr[i]);
        i++;
    }
    free(str_arr);
}

void free_shell_data(t_shell_data *data)
{
    free_env_list(&data->env);
    free_str_arr(data->paths);
}