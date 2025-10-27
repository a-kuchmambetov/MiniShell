/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:41:20 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 13:41:25 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/* ==================== helpers ==================== */

static int keys_equal(const char *a, const char *b)
{
    size_t la, lb;
    if (!a || !b) return 0;
    la = ft_strlen(a);
    lb = ft_strlen(b);
    return (la == lb) && (ft_strncmp(a, b, la) == 0);
}

static int env_key_exists(t_env_list *env, const char *key)
{
    t_env_node *cur = env ? env->first : NULL;
    while (cur)
    {
        if (keys_equal(cur->key, key))
            return 1;
        cur = cur->next;
    }
    return 0;
}

static char *strip_outer_quotes(const char *s)
{
    size_t n;
    if (!s) return NULL;
    n = ft_strlen(s);
    if (n >= 2 && ((s[0] == '"' && s[n-1] == '"') || (s[0] == '\'' && s[n-1] == '\'')))
        return ft_substr(s, 1, n - 2);
    return ft_strdup(s);
}

static int append_with_space(char **dst, const char *piece)
{
    char *old = *dst;
    char *sp  = ft_strjoin(old, " ");
    char *jn  = sp ? ft_strjoin(sp, piece) : NULL;
    free(old); free(sp);
    if (!jn) return 1;
    *dst = jn;
    return 0;
}

/* ==================== builtin_export ==================== */

int builtin_export(t_shell_data *data, char **args)
{
    int i = 1;
    int ret = 0;

    while (args[i])
    {
        char *eq = ft_strchr(args[i], '=');

        /* --- CASE 1: NAME=VALUE (одним токеном) --- */
        if (eq && eq != args[i] + ft_strlen(args[i]) - 1)
        {
            char *name = ft_substr(args[i], 0, (size_t)(eq - args[i]));
            if (!name) { ret = 1; ft_print_err("export: memory allocation error\n"); i++; continue; }

            if (!is_valid_identifier(name))
            {
                ft_print_err("export: `%s': not a valid identifier\n", name);
                free(name);
                ret = 1;
                i++;
                continue;
            }

            char *clean_val = strip_outer_quotes(eq + 1);
            if (!clean_val) { ret = 1; free(name); i++; continue; }

            char *tmp = ft_strjoin(name, "=");
            char *pair = tmp ? ft_strjoin(tmp, clean_val) : NULL;

            if (!tmp || !pair || !add_or_update_env(data, pair))
                ret = 1;

            free(tmp); free(pair); free(name); free(clean_val);
        }

        /* --- CASE 2: NAME= + VALUE в наступних токенах --- */
        else if (args[i][ft_strlen(args[i]) - 1] == '=')
        {
            size_t nlen = ft_strlen(args[i]) - 1;
            char *name = ft_strndup(args[i], nlen);
            if (!name) { ret = 1; i++; continue; }

            if (!is_valid_identifier(name))
            {
                ft_print_err("export: `%s': not a valid identifier\n", name);
                free(name);
                ret = 1;
                i++;
                continue;
            }

            int j = i + 1;
            char *value = ft_strdup(args[j] ? args[j] : "");
            if (!value) { ret = 1; free(name); i++; continue; }

            if (args[j] && (args[j][0] == '"' || args[j][0] == '\''))
            {
                char quote = args[j][0];
                while (args[j] && !(ft_strlen(args[j]) >= 2 && args[j][ft_strlen(args[j]) - 1] == quote))
                {
                    if (!args[++j]) break;
                    if (append_with_space(&value, args[j]))
                    {
                        free(name); free(value);
                        return 1;
                    }
                }
            }

            char *clean_val = strip_outer_quotes(value);
            char *tmp = clean_val ? ft_strjoin(name, "=") : NULL;
            char *pair = tmp ? ft_strjoin(tmp, clean_val) : NULL;

            if (!clean_val || !tmp || !pair || !add_or_update_env(data, pair))
                ret = 1;

            free(value); free(clean_val); free(tmp); free(pair); free(name);

            i = j;        // <-- КРИТИЧНО
            i++;          // цикл збільшить ще раз
            continue;     // <-- ГОЛОВНЕ: НЕ ДАТИ CASE3 виконатись
        }

        /* --- CASE 3: просто NAME без значення --- */
        else
        {
            if (!is_valid_identifier(args[i]))
            {
                ft_print_err("export: `%s': not a valid identifier\n", args[i]);
                ret = 1;
                i++;
                continue;
            }

            if (!env_key_exists(&data->env_list, args[i]))
                if (!add_or_update_env(data, args[i]))
                    ret = 1;
        }

        i++;
    }

    if (!sync_envp(data))
        ret = 1;

    data->last_exit_status = (ret ? 1 : 0) << 8;
    return ret;
}