/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:35:34 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 13:35:37 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Checks a string is a valid numeric argument for the `exit` builtin.
 *
 * Accepts optional leading whitespace, an optional sign, digits, and optional
 * trailing whitespace. Returns true only if the string contains at least one 
 * digit and contains no invalid characters.
 *
 * @param arg Input string.
 * @return true if the string represents a valid numeric argument, false 
 * otherwise.
 */
static bool	is_valid_numeric_arg(char *arg)
{
	int		i;
	bool	has_digits;

	i = 0;
	has_digits = false;
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
	return (has_digits);
}

/**
 * @brief Handles the case where `exit` is called with no argument or an empty 
 * string.
 *
 * - If no argument is provided, exits with the last exit status.
 * - If the argument is an empty string, exits with status 0.
 *
 * Frees all shell data before exiting.
 *
 * @param data Pointer to the shell data structure.
 * @param arg Argument string (may be NULL or an empty string).
 */
static void	handle_no_args_or_empty(t_shell_data *data, char *arg)
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
static void	handle_invalid_arg(t_shell_data *data, char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	free_shell_data(data);
	exit(2);
}

/**
 * @brief Implements the `exit` builtin command.
 *
 * Behavior:
 * - No argument → exit with last exit code.
 * - Empty argument → exit with 0.
 * - Non-numeric or overflow → print error and exit with 2.
 * - More than one argument → print error and return 1 (does NOT exit).
 * - Otherwise → exit with the parsed numeric value (cast to unsigned char).
 *
 * @param data Pointer to shell state.
 * @param args Array of command arguments (args[0] = "exit").
 * @return 1 if there are too many arguments (shell continues), otherwise the
 *         function does not return because the shell exits.
 */
int	builtin_exit(t_shell_data *data, char **args)
{
	bool	overflow;
	long	exit_value;

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
