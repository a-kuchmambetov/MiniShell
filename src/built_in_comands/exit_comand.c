#include "builtins.h"

// int builtin_exit(t_shell_data *data)
// {
//     if (data)
//         free_shell_data(data);
//     rl_clear_history();
//     exit(0);
// }
#include <limits.h>
#include <stdbool.h>

static bool	check_out_of_range(int neg, unsigned long long num, bool *error)
{
	if ((neg == 1 && num > LONG_MAX)
		|| (neg == -1 && num > -(unsigned long)LONG_MIN))
		*error = true;
	return (*error);
}

static long	ft_atoi_long(const char *str, bool *error)
{
	unsigned long long	num = 0;
	int					neg = 1;
	int					i = 0;

	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			neg = -1;
	while (str[i] && ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		if (check_out_of_range(neg, num, error))
			break ;
		i++;
	}
	return ((long)(num * neg));
}

static int	get_exit_code(char *arg, bool *error)
{
	int	i;

	if (!arg)
		return (0);
	i = 0;
	while (arg[i] == ' ')
		i++;
	if (arg[i] == '\0')
		*error = true;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!ft_isdigit(arg[i]))
		*error = true;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && arg[i] != ' ')
			*error = true;
		i++;
	}
	return ((int)(ft_atoi_long(arg, error) % 256));
}

int	builtin_exit(t_shell_data *data, char **args)
{
	bool	error;
	int		exit_code;

	error = false;
	ft_putendl_fd("exit", 1);
	if (!args[1]) 
	{
		free_shell_data(data);
		exit(data->last_exit_status);
	}
	exit_code = get_exit_code(args[1], &error);
	if (error)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_shell_data(data);
		exit(255);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	free_shell_data(data);
	exit(exit_code);
	return (exit_code);
}
