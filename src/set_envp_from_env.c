#include "../main.h"

void set_envp_from_env(t_shell_data *data)
{
    t_env_node *current;
    int i;

    free_str_arr(data->envp);
    current = data->env.first;
    data->envp = ft_calloc(sizeof(char *), (data->env.len + 1));
    i = 0;
    while (i < data->env.len)
    {
        data->envp[i] = ft_calloc(sizeof(char), (ft_strlen(current->key) + ft_strlen(current->value) + 2));
        if (!data->envp[i])
            return ;
        ft_strlcat(data->envp[i], current->key, ft_strlen(current->key) + 1);
        ft_strlcat(data->envp[i], "=", ft_strlen(data->envp[i]) + 2);
        ft_strlcat(data->envp[i], current->value, ft_strlen(data->envp[i]) + ft_strlen(current->value) + 1);
        current = current->next;
        i++;
    }
}
