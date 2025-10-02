#include "builtins.h"

int	builtin_env(t_shell_data *data, char **args)
{
	int	i;

	if (args[1])
	{
		ft_putstr_fd("minishell: env: No such file or directory\n", 2);
		return (1);
	}
	i = 0;
	while (data->envp[i])
	{
		if (ft_strchr(data->envp[i], '='))
			ft_printf("%s\n", data->envp[i]);
		i++;
	}
	return (0);
}
