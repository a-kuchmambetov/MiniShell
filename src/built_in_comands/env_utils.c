#include "builtins.h"

char	*get_env_value(char **envp, const char *name)
{
	int i = 0;
	int len = ft_strlen(name);

	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (NULL);
}

// int update_env(char **envp, const char *name, const char *value)
// {
//     int i = 0;
//     int len = ft_strlen(name);
//     char *new_var;

//     size_t total_len = len + ft_strlen(value) + 2;
//     new_var = malloc(total_len);
//     if (!new_var)
//         return 1;
//     ft_strlcpy(new_var, name, total_len);
//     ft_strlcat(new_var, "=", total_len);
//     ft_strlcat(new_var, value, total_len);

//     while (envp[i])
//     {
//         if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
//         {
//             free(envp[i]);
//             envp[i] = new_var;
//             return 0;
//         }
//         i++;
//     }

//     // без створення нового масиву — просто ігноруємо додавання
//     free(new_var);
//     return 0;
// }

// int update_env(char **envp, const char *name, const char *value)
// {
//     int i = 0;
//     int len = ft_strlen(name);
//     char *new_var;

//     while (envp[i])
//     {
//         if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
//         {
//             // створюємо рядок name=value
//             size_t total_len = len + ft_strlen(value) + 2;
//             new_var = malloc(total_len);
//             if (!new_var)
//                 return 1;

//             ft_strlcpy(new_var, name, total_len);
//             ft_strlcat(new_var, "=", total_len);
//             ft_strlcat(new_var, value, total_len);

//             free(envp[i]);   // звільняємо старий рядок
//             envp[i] = new_var;
//             return 0;
//         }
//         i++;
//     }

//     // якщо змінної не існує, нічого не додаємо
//     return 1;
// }

// Оновлює змінну, якщо вона є
int update_env(char **envp, const char *name, const char *value)
{
    int i = 0;
    int len = ft_strlen(name);
    char *new_var;
    
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
        {
            size_t total_len = len + ft_strlen(value) + 2;
            new_var = malloc(total_len);
            if (!new_var)
                return 1;
            ft_strlcpy(new_var, name, total_len);
            ft_strlcat(new_var, "=", total_len);
            ft_strlcat(new_var, value, total_len);

            free(envp[i]);
            envp[i] = new_var;
            return 0;
        }
        i++;
    }
    return 1; // змінної не було
}

// Додає нову змінну в кінець масиву
int add_env(char ***envp, const char *name, const char *value)
{
    int i = 0;
    while ((*envp)[i])
        i++;

    char **tmp = malloc(sizeof(char*) * (i + 2)); // +1 для нової, +1 для NULL
    if (!tmp)
        return 1;

    int j = 0;
    while (j < i)
    {
        tmp[j] = (*envp)[j];
        j++;
    }

    size_t total_len = ft_strlen(name) + ft_strlen(value) + 2;
    char *new_var = malloc(total_len);
    if (!new_var)
    {
        free(tmp);
        return 1;
    }
    ft_strlcpy(new_var, name, total_len);
    ft_strlcat(new_var, "=", total_len);
    ft_strlcat(new_var, value, total_len);

    tmp[j++] = new_var;
    tmp[j] = NULL;

    free(*envp);
    *envp = tmp;
    return 0;
}




char *ft_strjoin_free(char *s1, const char *sep, const char *s2)
{
    size_t len = ft_strlen(s1) + ft_strlen(sep) + ft_strlen(s2) + 1;
    char *res = malloc(len);
    if (!res)
        return (NULL);
    ft_strlcpy(res, s1, len);
    ft_strlcat(res, sep, len);
    ft_strlcat(res, s2, len);
    free(s1);
    return (res);
}
