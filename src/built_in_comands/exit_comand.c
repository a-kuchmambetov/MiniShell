#include "builtins.h"

/**
 * @brief Checks if a number is out of long range.
 *
 * @param neg Sign of the number (1 or -1).
 * @param num The numeric value to check.
 * @param error Pointer to error flag, set to true if out of range.
 * @return true if out of range, false otherwise.
 */
static bool	check_out_of_range(int neg, unsigned long long num, bool *error)
{
	if (neg == 1)
    {
        if (num > LONG_MAX)
            *error = true;
    }
    else
    {
        if (num > -(unsigned long)LONG_MIN)
            *error = true;
    }
    return (*error);
}
/**
 * @brief Converts a string to long with overflow detection.
 *
 * @param str Input string.
 * @param error Pointer to overflow error flag.
 * @return Converted long value.
 */
static long	ft_atoi_long(const char *str, bool *overflow)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	neg = 1;
	i = 0;
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
		}
		i++;
	}
	return ((long)(num * neg));
}

/**
 * @brief Checks if a string is a valid numeric argument for exit.
 *
 * @param arg Input string.
 * @return true if valid, false otherwise.
 */
static bool	is_valid_numeric_arg(char *arg)
{
	int i;
	bool has_digits;

	i = 0;
	has_digits = false;
	if (!arg)
		return (false);
	while (arg[i] == ' ' || (arg[i] >= 9 && arg[i] <= 13))
		return (false);
	while (arg[i] == ' ' || (arg[i] >= 9 && arg[i] <= 13))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
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
	return (has_digits);
}
/**
 * @brief Handles exit when argument is missing or empty string.
 *
 * If no argument is provided, exits with last exit status.
 * If argument is an empty string, exits with status 0.
 *
 * @param data Pointer to shell data structure.
 * @param arg Argument string (may be NULL or empty).
 */
static void handle_no_args_or_empty(t_shell_data *data, char *arg)
{
    free_shell_data(data);
    if (!arg)
        exit(data->last_exit_status);
    exit(0);
}

/**
 * @brief Handles invalid numeric argument or overflow for 'exit'.
 *
 * Prints an error message and exits with status 2.
 *
 * @param data Pointer to shell data structure.
 * @param arg Argument string.
 */
static void handle_invalid_arg(t_shell_data *data, char *arg)
{
    ft_putstr_fd("minishell: exit: ", 2);
    ft_putstr_fd(arg, 2);
    ft_putendl_fd(": numeric argument required", 2);
    free_shell_data(data);
    exit(2);
}

/**
 * @brief Implements the 'exit' builtin command.
 *
 * Handles numeric argument, overflow, empty argument, and too many arguments.
 *
 * @param data Pointer to shell data structure.
 * @param args Command arguments array.
 * @return Returns 1 if too many arguments, otherwise exits the program.
 */
int builtin_exit(t_shell_data *data, char **args)
{
    bool overflow;
    long exit_value;

	overflow = false;
    ft_putendl_fd("exit", 1);
    if (!args[1] || args[1][0] == '\0')
        handle_no_args_or_empty(data, args[1]);
    if (!is_valid_numeric_arg(args[1]))
        handle_invalid_arg(data, args[1]);
    exit_value = ft_atoi_long(args[1], &overflow);
    if (overflow)
        handle_invalid_arg(data, args[1]);
    if (args[2])
    {
        ft_putendl_fd("minishell: exit: too many arguments", 2);
        data->last_exit_status = 1;
        return (1);
    }
    free_shell_data(data);
    exit((unsigned char)exit_value);
}


