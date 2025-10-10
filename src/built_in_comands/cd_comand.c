#include "builtins.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	res = malloc(len1 + len2 + len3 + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	ft_memcpy(res + len1 + len2, s3, len3);
	res[len1 + len2 + len3] = '\0';
	return (res);
}

static char *join_paths(const char *base, const char *rel)
{
    char *joined;
    if (!rel || !*rel)
        return ft_strdup(base);
    if (rel[0] == '/')
        return ft_strdup(rel); // абсолютний шлях
    joined = ft_strjoin3(base, "/", rel);
    return joined;
}

static char *normalize_path(const char *path)
{
    char *parts[4096];
    char *token;
    char *copy = ft_strdup(path);
    int i = 0;

    token = strtok(copy, "/");
    while (token)
    {
        if (ft_strncmp(token, "..", 3) == 0)
        {
            if (i > 0)
                i--;
        }
        else if (ft_strncmp(token, ".", 2) != 0 && *token != '\0')
        {
            parts[i++] = token;
        }
        token = strtok(NULL, "/"); //zaboronena funktsia
    }

    char *result = ft_strdup("/");
    for (int j = 0; j < i; j++)
    {
        char *tmp = ft_strjoin(result, parts[j]);
        free(result);
        if (j < i - 1)
        {
            char *tmp2 = ft_strjoin(tmp, "/");
            free(tmp);
            tmp = tmp2;
        }
        result = tmp;
    }
    free(copy);
    return result;
}

int builtin_cd(t_shell_data *data, char **args)
{
    char *target = NULL;
    char *new_path;
    char *oldpwd;

    oldpwd = ft_strdup(data->pwd ? data->pwd : getcwd(NULL, 0));
    if (args[1] == NULL || ft_strncmp(args[1], "~", 2) == 0)
        target = ft_strdup(getenv("HOME"));
    else if (ft_strncmp(args[1], "-", 2) == 0)
    {
        if (data->oldpwd)
        {
            ft_printf("%s\n", data->oldpwd);
            target = ft_strdup(data->oldpwd);
        }
        else
        {
            ft_printf("cd: OLDPWD not set\n");
            free(oldpwd);
            return 1;
        }
    }
    else
        target = ft_strdup(args[1]);

    new_path = join_paths(data->pwd, target);
    free(target);
    char *normalized = normalize_path(new_path);
    free(new_path);

    if (access(normalized, F_OK) != 0)
    {
        ft_printf("cd: no such file or directory: %s\n", normalized);
        free(normalized);
        free(oldpwd);
        return 1;
    }

    if (chdir(normalized) != 0)
    {
        perror("cd");
        free(normalized);
        free(oldpwd);
        return 1;
    }

    free(data->oldpwd);
    data->oldpwd = oldpwd;
    free(data->pwd);
    data->pwd = normalized;

    add_or_update_env(data, ft_strjoin("PWD=", data->pwd));
    add_or_update_env(data, ft_strjoin("OLDPWD=", data->oldpwd));
    sync_envp(data);

    return 0;
}
// #include "builtins.h"

// static char	*get_target_path(t_shell_data *data, char **args)
// {
// 	char	*value;

// 	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
// 	{
// 		value = get_env_value(data->envp, "HOME");
// 		if (value)
// 			return (ft_strdup(value));
// 		return (NULL);
// 	}
// 	if (ft_strncmp(args[1], "-", 2) == 0)
// 	{
// 		value = get_env_value(data->envp, "OLDPWD");
// 		if (value)
// 			return (ft_strdup(value));
// 		return (NULL);
// 	}
// 	return (ft_strdup(args[1]));
// }
// #include <sys/stat.h>

// int is_dir(const char *path)
// {
//     struct stat st;
//     if (stat(path, &st) != 0)
//         return 0;
//     return S_ISDIR(st.st_mode);
// }
// char *get_safe_cwd(t_shell_data *data)
// {
//     char *cwd = data->pwd;

//     if (!is_dir(cwd) || access(cwd, F_OK) != 0)
//     {
//         free(cwd);
//         cwd = ft_strdup(data->last_valid_dir);
//         chdir(cwd); // повертаємось у останній валідний каталог
//     }

//     free(data->last_valid_dir);
//     data->last_valid_dir = ft_strdup(cwd);
//     return cwd;
// }
// int builtin_cd(t_shell_data *data, char **args)
// {
//     char *path;
//     char *oldpwd;
//     char *tmp;
//     char *new_pwd;
//     char *slash;

//     // Перевірка кількості аргументів
//     if (args[2])
//     {
//         ft_printf("cd: too many arguments\n");
//         return 1;
//     }

//     // Зберігаємо OLD PWD
//     oldpwd = ft_strdup(data->pwd);
//     ft_printf("[DEBUG] OLD PWD: %s\n", oldpwd);

//     // Отримуємо target path
//     path = get_target_path(data, args);
//     if (!path)
//     {
//         ft_printf("cd: path not set\n");
//         free(oldpwd);
//         return 1;
//     }
//     ft_printf("[DEBUG] Target path: %s\n", path);

//     // Обробка ".."
//     if (ft_strncmp(path, "..", 3) == 0)
//     {
//         slash = ft_strrchr(data->pwd, '/');
//         if (slash && slash != data->pwd)
//             *slash = '\0';
//         else
//             ft_strcpy(data->pwd, "/");
//     }
//     else if (ft_strncmp(path, "-", 2) == 0)
//     {
//         // Повертаємося у OLDPWD
//         free(path);
//         path = ft_strdup(oldpwd);
//         ft_strcpy(data->pwd, path);
//     }
//     else if (ft_strncmp(path, "~", 2) == 0)
//     {
//         // HOME
//         free(path);
//         path = ft_strdup(data->home_dir);
//         ft_strcpy(data->pwd, path);
//     }
//     else
//     {
//         // Додаємо новий компонент до pwd
//         tmp = ft_strjoin(data->pwd, "/");
//         new_pwd = ft_strjoin(tmp, path);
//         free(tmp);
//         free(data->pwd);
//         data->pwd = new_pwd;
//     }

//     // Перевірка, чи існує фізично каталог
//     if (!is_dir(data->pwd))
//     {
//         ft_printf("cd: directory does not exist, staying in %s\n", oldpwd);
//         free(data->pwd);
//         data->pwd = oldpwd;
//         free(path);
//         return 1;
//     }

//     // Викликаємо chdir для фізичного каталогу (sandbox-safe)
//     if (chdir(data->pwd) != 0)
//     {
//         perror("cd");
//         free(oldpwd);
//         free(path);
//         return 1;
//     }

//     // Оновлюємо PWD та OLDPWD у env
//     tmp = ft_strjoin("OLDPWD=", oldpwd);
//     add_or_update_env(data, tmp);
//     free(tmp);
//     free(oldpwd);

//     tmp = ft_strjoin("PWD=", data->pwd);
//     add_or_update_env(data, tmp);
//     free(tmp);

//     sync_envp(data);

//     if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
//         ft_printf("%s\n", data->pwd);

//     free(path);
//     return 0;
// }


// // int	builtin_cd(t_shell_data *data, char **args)
// // {
// // 	char	*path;
// // 	char	*oldpwd;
// // 	char	*cwd;
// // 	char	*tmp;

// // 	while (args[2] && args[2])
// // 	{
// // 		ft_printf("cd: too many arguments\n");
// // 		return (1);
// // 	}
// // 	oldpwd = ft_strdup(data->pwd);
// //     // if (!oldpwd || access(oldpwd, F_OK) != 0)
// //     // {
// //     //     free(oldpwd);
// //     //     oldpwd = ft_strdup(data->last_valid_dir);
// //     //     chdir(oldpwd); // повертаємось у останній валідний
// //     // }
// //     // free(data->last_valid_dir);
// //     // data->last_valid_dir = ft_strdup(oldpwd);
// //     ft_printf("[DEBUG] OLD PWD: %s\n", oldpwd);


// // 	path = get_target_path(data, args);
// // 	if (!path)
// // 	{
// //         ft_printf("cd: path not set\n");
// //         free(oldpwd);
// //         return 1;
// //     }
// // 	ft_printf("[DEBUG] Target path: %s\n", path);

// // 	if (!is_dir(path) || access(path, F_OK) != 0)
// //     {
// //         ft_printf("cd: directory does not exist, returning to last valid directory\n");
// //         free(path);
// //         path = ft_strdup(data->last_valid_dir);
// //     }
// // 	if (chdir(path) != 0)
// //     {
// //         perror("cd");
// //         free(path);
// //         free(oldpwd);
// //         return 1;
// //     }
// // 	cwd = get_safe_cwd(data);
// // 	if (!cwd)
// // 	{
// // 		perror("getcwd");
// // 		free(path);
// // 		free(oldpwd);
// // 		return (1);
// // 	}
// // 	free(data->last_valid_dir);
// //     data->last_valid_dir = ft_strdup(cwd);
// //     ft_printf("[DEBUG] New CWD: %s\n", cwd);

// // 	tmp = ft_strjoin("OLDPWD=", oldpwd);
// // 	add_or_update_env(data, tmp);
// // 	free(tmp);
// // 	free(oldpwd);
// // 	tmp = ft_strjoin("PWD=", cwd);
// // 	add_or_update_env(data, tmp);
// // 	free(tmp);
// // 	sync_envp(data);
// // 	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
// // 		ft_printf("%s\n", cwd);
// // 	free(cwd);
// // 	free(path);
// // 	return (0);
// // }



// // char *get_safe_cwd(t_shell_data *data)
// // {
// //     char *cwd = getcwd(NULL, 0);

// //     if (!is_dir(cwd) || access(cwd, F_OK) != 0)
// //     {
// //         free(cwd);
// //         cwd = ft_strdup(data->last_valid_dir);
// //         chdir(cwd); // повертаємось у останній валідний каталог
// //     }

// //     free(data->last_valid_dir);
// //     data->last_valid_dir = ft_strdup(cwd);
// //     return cwd;
// // }

// // int	builtin_cd(t_shell_data *data, char **args)
// // {
// // 	char	*path;
// // 	char	*oldpwd;
// // 	char	*cwd;
// // 	char	*tmp;

// // 	while (args[1] && args[2])
// // 	{
// // 		ft_printf("cd: too many arguments\n");
// // 		return (1);
// // 	}
// // 	oldpwd = get_safe_cwd(data);
 
// //     ft_printf("[DEBUG] OLD PWD: %s\n", oldpwd);

// // 	path = get_target_path(data, args);
// // 	if (!path)
// // 	{
// //         ft_printf("cd: path not set\n");
// //         free(oldpwd);
// //         return (1);
// //     }
// // 	ft_printf("[DEBUG] Target path: %s\n", path);

// // 	if (chdir(path) != 0)
// //     {
// //         perror("cd");
// //         free(path);
// //         free(oldpwd);
// //         return (1);
// //     }

// // 	cwd = get_safe_cwd(data);

// // 	ft_printf("[DEBUG] New CWD: %s\n", cwd);

// // 	tmp = ft_strjoin("OLDPWD=", oldpwd);
// // 	add_or_update_env(data, tmp);
// // 	free(tmp);
// // 	free(oldpwd);
	
// // 	tmp = ft_strjoin("PWD=", cwd);
// // 	add_or_update_env(data, tmp);
// // 	free(tmp);
// // 	sync_envp(data);
// // 	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
// // 		ft_printf("%s\n", cwd);
// // 	free(cwd);
// // 	free(path);
// // 	return (0);
// // }