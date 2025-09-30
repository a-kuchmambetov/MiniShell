#include "builtins.h"

static char	*get_target_path(t_shell_data *data, char **args)
{
	char	*value;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
	{
		value = get_env_value(data->envp, "HOME");
		if (value)
			return (ft_strdup(value));
		return (NULL);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		value = get_env_value(data->envp, "OLDPWD");
		if (value)
			return (ft_strdup(value));
		return (NULL);
	}
	return (ft_strdup(args[1]));
}

int	builtin_cd(t_shell_data *data, char **args)
{
	int		i;
	char	*path;
	char	*oldpwd_val;
	char	*oldpwd;
	char	*cwd;
	char	*tmp;

	i = 0;
	while (args[i])
		i++;
	if (i > 2)
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	path = get_target_path(data, args);
	if (!path)
		return (ft_printf("cd: path not set\n"), 1);
	oldpwd_val = get_env_value(data->envp, "PWD");
	if (oldpwd_val)
		oldpwd = ft_strdup(oldpwd_val);
	else
		oldpwd = ft_strdup("");
	if (chdir(path) != 0)
	{
		perror("cd");
		free(path);
		free(oldpwd);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		free(path);
		free(oldpwd);
		return (1);
	}
	tmp = ft_strjoin("OLDPWD=", oldpwd);
	add_or_update_env(data, tmp);
	free(tmp);
	free(oldpwd);
	tmp = ft_strjoin("PWD=", cwd);
	add_or_update_env(data, tmp);
	free(tmp);
	sync_envp(data);
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_printf("%s\n", cwd);
	free(cwd);
	free(path);
	return (0);
}

