#include "builtins.h"

static int	parse_pwd_flag(char **args)
{
	if (!args[1])
		return (0);
	if (args[1][0] == '-' && args[1][1] && !args[1][2])
	{
		if (args[1][1] == 'P')
			return (1);
		else if (args[1][1] == 'L')
			return (0);
		fprintf(stderr, "pwd: invalid option: %s\n", args[1]);
		return (-1);
	}
	return (0);
}

int	builtin_pwd(t_shell_data *data, char **args)
{
	char	*pwd;
	char	*cwd;
	int		phys;

	(void)data;
	phys = parse_pwd_flag(args);
	if (phys == -1)
		return (1);
	if (!phys)
	{
		pwd = get_env_value(data->envp, "PWD");
		if (pwd && *pwd)
		{
			printf("%s\n", pwd);
			return (0);
		}
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
