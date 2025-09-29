#include "builtins.h"

static char *get_target_path(t_shell_data *data, char **args)
{
    if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
        return get_env_value(data->envp, "HOME") ? ft_strdup(get_env_value(data->envp, "HOME")) : NULL;
    if (ft_strncmp(args[1], "-", 2) == 0)
        return get_env_value(data->envp, "OLDPWD") ? ft_strdup(get_env_value(data->envp, "OLDPWD")) : NULL;
    return ft_strdup(args[1]);
}

int builtin_cd(t_shell_data *data, char **args)
{
    int arg_count = 0;
    while (args[arg_count])
        arg_count++;

    if (arg_count > 2)
    {
        ft_printf("cd: too many arguments\n");
        return 1;
    }

    // отримуємо шлях для переходу
    char *path = get_target_path(data, args);
    if (!path)
        return (ft_printf("cd: path not set\n"), 1);

    // зберігаємо старий PWD
    char *oldpwd = get_env_value(data->envp, "PWD");
    if (!oldpwd)
        oldpwd = "";

    // зміна директорії
    if (chdir(path) != 0)
    {
        perror("cd");
        free(path);
        return 1;
    }

    // отримуємо новий абсолютний шлях
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("getcwd");
        free(path);
        return 1;
    }

    // оновлюємо OLDPWD через add_or_update_env
    char *oldpwd_arg = ft_strjoin("OLDPWD=", oldpwd);
    add_or_update_env(data, oldpwd_arg);
    free(oldpwd_arg);

    // оновлюємо PWD через add_or_update_env
    char *cwd_arg = ft_strjoin("PWD=", cwd);
    add_or_update_env(data, cwd_arg);
    free(cwd_arg);

    // синхронізуємо envp з t_env_list
    sync_envp(data);

    // якщо cd -, виводимо новий шлях
    if (args[1] && strcmp(args[1], "-") == 0)
        ft_printf("%s\n", cwd);

    free(cwd);
    free(path);
    return 0;
}
