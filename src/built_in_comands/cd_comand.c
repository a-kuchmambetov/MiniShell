#include "builtins.h"
#include <errno.h>

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
static void update_env_pwd(t_shell_data *data, char *key, char *value)
{
    char *tmp = ft_strjoin(key, value);
    add_or_update_env(data, tmp);
    free(tmp);
}

int builtin_cd(t_shell_data *data, char **args)
{
	char	*path;
	char	*oldpwd;
	char	*cwd;

	if (args[2])
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd");
		return (1);
	}
	path = get_target_path(data, args);
	if (!path)
	{
		free(oldpwd);
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_printf("cd: %s: %s\n", path, strerror(errno));
		free(oldpwd);
		free(path);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell:  getcwd");
        cwd = ft_strdup(data->pwd ? data->pwd : "/");
	}
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_printf("%s\n", cwd);
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
