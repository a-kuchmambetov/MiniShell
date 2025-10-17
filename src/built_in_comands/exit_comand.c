#include "builtins.h"

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

static bool	is_valid_numeric_arg(char *arg)
{
	int i = 0;
	bool has_digits = false;

	if (!arg)
		return (false);
	while (arg[i] == ' ' || (arg[i] >= 9 && arg[i] <= 13))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]))
			has_digits = true;
		else if (arg[i] != ' ' && !(arg[i] >= 9 && arg[i] <= 13))
			return (false);
		i++;
	}
	return (has_digits); // повертає false, якщо цифр нема взагалі
}

int	builtin_exit(t_shell_data *data, char **args)
{
	bool	overflow = false;
	long	exit_value;

	ft_putendl_fd("exit", 1);

	// No arguments
	if (!args[1])
	{
		free_shell_data(data);
		exit(data->last_exit_status);
	}

	// Empty string or only spaces should be treated as no argument
	if (args[1] && args[1][0] == '\0')
	{
		free_shell_data(data);
		exit(0);
	}

	// Invalid numeric argument or overflow
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

	// Too many arguments
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		data->last_exit_status = 1;
		return (1); // НЕ exit(), як у Bash
	}

	free_shell_data(data);
	exit((unsigned char)exit_value);
}

