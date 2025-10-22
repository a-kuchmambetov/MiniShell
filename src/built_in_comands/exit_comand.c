#include "builtins.h"



static bool	check_out_of_range(int neg, unsigned long long num)
{
	if ((neg == 1 && num > LONG_MAX)
		|| (neg == -1 && num > -(unsigned long)LONG_MIN))
		return (true);
	return (false);
}

static long	ft_atoi_long(const char *str, bool *overflow)
{
	unsigned long long	num = 0;
	int					neg = 1;
	int					i = 0;

	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			neg = -1;
	while (str[i] && ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		if (check_out_of_range(neg, num))
		{
			*overflow = true;
			break ;
		}
		i++;
	}
	return ((long)(num * neg));
}

static bool	is_valid_numeric_arg(char *arg)
{
	int i = 0;

	if (!arg)
		return (false);
	while (arg[i] == ' ' || (arg[i] >= 9 && arg[i] <= 13))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!ft_isdigit(arg[i]))
		return (false);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && arg[i] != ' ')
			return (false);
		i++;
	}
	return (true);
}

int	builtin_exit(t_shell_data *data, char **args)
{
	bool	overflow = false;
	long	exit_value;

	ft_putendl_fd("exit", 1);
	if (!args[1])
	{
		free_shell_data(data);
		exit(data->last_exit_status);
	}
	if (!is_valid_numeric_arg(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_shell_data(data);
		exit(2);
	}
	exit_value = ft_atoi_long(args[1], &overflow);
	if (overflow)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_shell_data(data);
		exit(2);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		data->last_exit_status = 1;
		return (1);
	}
	free_shell_data(data);
	exit((unsigned char)exit_value);
}
