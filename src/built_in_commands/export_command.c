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

/* ===== KEYS ===== */
static int	env_key_exists(t_env_list *env, const char *key)
{
	t_env_node	*cur;
	size_t		len_key;
	size_t		len_cur;

	if (!env || !key)
		return (0);
	cur = env->first;
	len_key = ft_strlen(key);
	while (cur)
	{
		len_cur = ft_strlen(cur->key);
		if (len_key == len_cur && ft_strncmp(cur->key, key, len_key) == 0)
			return (1);
		cur = cur->next;
	}
	return (0);
}


/* ===== STRING HELPERS ===== */
static char	*strip_outer_quotes(const char *s)
{
	size_t	n;

	if (!s)
		return (NULL);
	n = ft_strlen(s);
	if (n >= 2 && ((s[0] == '"' && s[n - 1] == '"')
			|| (s[0] == '\'' && s[n - 1] == '\'')))
		return (ft_substr(s, 1, n - 2));
	return (ft_strdup(s));
}

static int	append_with_space(char **dst, const char *piece)
{
	char	*old;
	char	*sp;
	char	*jn;

	old = *dst;
	sp = ft_strjoin(old, " ");
	if (!sp)
		return (1);
	jn = ft_strjoin(sp, piece);
	free(old);
	free(sp);
	if (!jn)
		return (1);
	*dst = jn;
	return (0);
}

/* ===== ADD PAIR TO ENV ===== */
static int	add_pair_to_env(t_shell_data *data, char *name, char *clean_val)
{
	char	*tmp;
	char	*pair;
	int		ret;

	ret = 0;
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (1);
	pair = ft_strjoin(tmp, clean_val);
	if (!pair)
		ret = 1;
	else if (!add_or_update_env(data, pair))
		ret = 1;
	free(tmp);
	free(pair);
	return (ret);
}

/* ===== CASE 1: NAME=VALUE ===== */
static int	handle_full_assignment(t_shell_data *data, char *arg)
{
	char	*eq;
	char	*name;
	char	*clean_val;

	eq = ft_strchr(arg, '=');
	name = ft_substr(arg, 0, (size_t)(eq - arg));
	if (!name)
		return (ft_print_err("export: memory allocation error\n"), 1);
	if (!is_valid_identifier(name))
	{
		ft_print_err("export: `%s': not a valid identifier\n", name);
		return (free(name),1);
	}
	clean_val = strip_outer_quotes(eq + 1);
	if (!clean_val)
		return (free(name), 1);
	if (add_pair_to_env(data, name, clean_val))
	{
		free(name);
		free(clean_val);
		return (1);
	}
	free(name);
	free(clean_val);
	return (0);
}

/* ===== JOIN QUOTED VALUE ===== */
static int	join_quoted_value(char **value, char **args, int *j)
{
	char	quote;

	quote = args[*j][0];
	while (args[*j] && !(ft_strlen(args[*j]) >= 2
			&& args[*j][ft_strlen(args[*j]) - 1] == quote))
	{
		(*j)++;
		if (!args[*j])
			break ;
		if (append_with_space(value, args[*j]))
			return (1);
	}
	return (0);
}

/* ===== CASE 2: NAME= + VALUE ===== */
static int	handle_split_assignment(t_shell_data *data, char **args, int *i)
{
	size_t	nlen;
	char	*name;
	char	*value;
	char	*clean_val;
	int		j;
	int		ret;

	nlen = ft_strlen(args[*i]) - 1;
	name = ft_strndup(args[*i], nlen);
	if (!name)
		return (1);
	if (!is_valid_identifier(name))
	{
		ft_print_err("export: `%s': not a valid identifier\n", name);
		free(name);
		return (1);
	}
	j = *i + 1;
	value = ft_strdup(args[j] ? args[j] : "");
	if (!value)
		return (free(name), 1);
	if (args[j] && (args[j][0] == '"' || args[j][0] == '\''))
		if (join_quoted_value(&value, args, &j))
			return (free(name), free(value), 1);
	clean_val = strip_outer_quotes(value);
	ret = add_pair_to_env(data, name, clean_val);
	free(name);
	free(value);
	free(clean_val);
	*i = j;
	return (ret);
}

/* ===== CASE 3: only NAME ===== */
static int	handle_name_only(t_shell_data *data, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_print_err("export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	if (!env_key_exists(&data->env_list, arg))
	{
		if (!add_or_update_env(data, arg))
			return (1);
	}
	return (0);
}

//* ===== UPDATE EXIT STATUS ===== */
static void	update_exit_status(t_shell_data *data, int ret)
{
	if (ret != 0)
		data->last_exit_status = 256;
	else
		data->last_exit_status = 0;
}

static void	process_export_arg(t_shell_data *data, char **args, int *i, int *ret)
{
	char	*eq;

	eq = ft_strchr(args[*i], '=');
	if (eq && eq != args[*i] + ft_strlen(args[*i]) - 1)
	{
		if (handle_full_assignment(data, args[*i]))
			*ret = 1;
	}
	else if (args[*i][ft_strlen(args[*i]) - 1] == '=')
	{
		if (handle_split_assignment(data, args, i))
			*ret = 1;
	}
	else
	{
		if (handle_name_only(data, args[*i]))
			*ret = 1;
	}
}

/* ===== MAIN BUILTIN EXPORT ===== */
int	builtin_export(t_shell_data *data, char **args)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
    int k = 0;
    while (args[k])
    {
        printf("DEBUG ARG[%d] = <%s>\n", k, args[k]);
        k++;
    }

	while (args[i])
	{
		process_export_arg(data, args, &i, &ret);
		i++;
	}
	if (!sync_envp(data))
		ret = 1;
	update_exit_status(data, ret);
	return (ret);
}
