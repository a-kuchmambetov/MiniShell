#include "builtins.h"
#include <errno.h>

/**
 * @brief Returns the target path for the cd command.
 *
 * Handles special cases:
 * - "~" or no argument → HOME directory
 * - "-" → OLDPWD directory
 * Otherwise, returns the argument as the path.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array.
 * @return Duplicated string of the target path, or NULL on error.
 */
static char	*get_target_path(t_shell_data *data, char **args)
{
	char	*value;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
	{
		value = get_env_value(data->envp, "HOME");
		if (value)
			return (ft_strdup(value));
        ft_printf("cd: HOME not set\n");
		return (NULL);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		value = get_env_value(data->envp, "OLDPWD");
		if (value)
			return (ft_strdup(value));
        ft_printf("cd: OLDPWD not set\n");
		return (NULL);
	}
	return (ft_strdup(args[1]));
}

/**
 * @brief Updates the environment variable with the new PWD or OLDPWD.
 *
 * Concatenates the key and value, then updates or adds it to envp.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param key The environment variable key (e.g., "PWD=" or "OLDPWD=").
 * @param value The new value to set.
 */
static void update_env_pwd(t_shell_data *data, char *key, char *value)
{
    char *tmp = ft_strjoin(key, value);
    add_or_update_env(data, tmp);
    free(tmp);
}

/**
 * @brief Validates cd command arguments (no more than one argument allowed).
 *
 * @param args Command arguments array.
 * @return 1 if too many arguments, 0 otherwise.
 */
static int	validate_cd_args(char **args)
{
	if (args[2])
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	return (0);
}

/**
 * @brief Gets old working directory and target path for cd.
 *
 * Allocates oldpwd using getcwd and retrieves the target path using get_target_path.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array.
 * @param oldpwd Pointer to store the old working directory string.
 * @return Duplicated string of the target path, or NULL on error.
 */
static char	*get_old_and_target(t_shell_data *data, char **args, char **oldpwd)
{
	char	*path;

	*oldpwd = getcwd(NULL, 0);
	if (!*oldpwd)
	{
		perror("getcwd");
		return (NULL);
	}
	path = get_target_path(data, args);
	if (!path)
	{
		free(*oldpwd);
		return (NULL);
	}
	return (path);
}

/**
 * @brief Gets current working directory, or fallback "/" if getcwd fails.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @return Duplicated string of the current working directory.
 */
static char	*get_current_dir_or_fallback(t_shell_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: getcwd");
		cwd = ft_strdup(data->pwd ? data->pwd : "/");
	}
	return (cwd);
}

/**
 * @brief Prints the current directory if cd argument is "-".
 *
 * @param args Command arguments array.
 * @param cwd Current working directory string.
 */
static void	print_dash_if_needed(char **args, char *cwd)
{
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_printf("%s\n", cwd);
}

/**
 * @brief Changes directory and updates environment variables PWD and OLDPWD.
 *
 * Handles error printing and memory management for oldpwd and path.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param path Target directory path.
 * @param oldpwd Previous working directory string.
 * @param args Command arguments array.
 * @return 0 on success, 1 on failure.
 */
static int	change_dir_and_update(t_shell_data *data, char *path, char *oldpwd, char **args)
{
	char	*cwd;

	if (chdir(path) != 0)
	{
		ft_printf("cd: %s: %s\n", path, strerror(errno));
		free(oldpwd);
		free(path);
		return (1);
	}
	cwd = get_current_dir_or_fallback(data);
	print_dash_if_needed(args, cwd);
	update_env_pwd(data, "OLDPWD=", oldpwd);
	update_env_pwd(data, "PWD=", cwd);
	sync_envp(data);
	free(data->pwd);
	data->pwd = ft_strdup(cwd);
	free(oldpwd);
	free(cwd);
	free(path);
	return (0);
}
/**
 * @brief Executes the builtin cd command.
 *
 * Validates arguments, determines the target path, and performs directory change.
 *
 * @param data Pointer to shell data structure containing environment info.
 * @param args Command arguments array.
 * @return 0 on success, 1 on failure.
 */
int	builtin_cd(t_shell_data *data, char **args)
{
	char	*oldpwd;
	char	*path;

	if (validate_cd_args(args))
		return (1);
	path = get_old_and_target(data, args, &oldpwd);
	if (!path)
		return (1);
	return (change_dir_and_update(data, path, oldpwd, args));
}
