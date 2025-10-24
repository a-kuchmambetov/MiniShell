/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:57:56 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 12:58:56 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
 * @brief Appends a character to parser buffer, reallocating if needed.
 * @param p Pointer to parser structure.
 * @param c Character to append.
 * @return 1 on success, 0 on allocation failure.
 */
static int	append_char(t_parser *p, char c)
{
	char	*newbuf;
	int		newsize;

	if (p->pos + 1 >= p->bufsize)
	{
		newsize = p->bufsize * 2;
		newbuf = malloc(newsize);
		if (!newbuf)
			return (0);
		ft_memcpy(newbuf, p->res, p->pos);
		free(p->res);
		p->res = newbuf;
		p->bufsize = newsize;
	}
	p->res[p->pos++] = c;
	return (1);
}

/**
 * @brief Expands environment variable in a string starting with '$'.
 * @param p Pointer to parser structure.
 */
static void	expand_var(t_parser *p)
{
	int		start;
	int		len;
	int		j;
	char	var[128];
	char	*val;

	start = ++p->i;
	while ((p->s[p->i] >= 'a' && p->s[p->i] <= 'z')
		|| (p->s[p->i] >= 'A' && p->s[p->i] <= 'Z')
		|| (p->s[p->i] >= '0' && p->s[p->i] <= '9')
		|| p->s[p->i] == '_')
		p->i++;
	len = p->i - start;
	if (len >= 128)
		len = 127;
	ft_strlcpy(var, p->s + start, len + 1);
	val = NULL;
	j = -1;
	while (p->envp && p->envp[++j])
	{
		if (!ft_strncmp(p->envp[j], var, len) && p->envp[j][len] == '=')
			val = p->envp[j] + len + 1;
	}
	while (val && *val)
		append_char(p, *val++);
}

/**
 * @brief Parses string enclosed in single quotes.
 * @param p Pointer to parser structure.
 */
void	parse_single_quotes(t_parser *p)
{
	char	quote;

	quote = p->s[p->i++];
	while (p->s[p->i] && p->s[p->i] != quote)
		append_char(p, p->s[p->i++]);
	if (p->s[p->i] == quote)
		p->i++;
}

/**
 * @brief Parses double quotes, variable expansions, or plain text.
 * @param p Pointer to parser structure.
 */
void	parse_double_or_plain(t_parser *p)
{
	char	quote;

	if (p->s[p->i] == '"')
	{
		quote = p->s[p->i++];
		while (p->s[p->i] && p->s[p->i] != quote)
		{
			if (p->s[p->i] == '$')
				expand_var(p);
			else
				append_char(p, p->s[p->i++]);
		}
		if (p->s[p->i] == quote)
			p->i++;
	}
	else if (p->s[p->i] == '$')
		expand_var(p);
	else
		append_char(p, p->s[p->i++]);
}

/**
 * @brief Parses a single argument string, expanding quotes and variables.
 * @param s Input string.
 * @param envp Environment variables.
 * @return Allocated parsed string, or NULL on failure.
 */
char	*parse_arg(const char *s, char **envp)
{
	t_parser	p;

	if (!s)
		return (NULL);
	p.bufsize = 64;
	p.res = malloc(p.bufsize);
	if (!p.res)
		return (NULL);
	p.s = s;
	p.envp = envp;
	p.pos = 0;
	p.i = 0;
	while (p.s[p.i])
	{
		if (p.s[p.i] == '\'')
			parse_single_quotes(&p);
		else
			parse_double_or_plain(&p);
	}
	if (!append_char(&p, '\0'))
	{
		free(p.res);
		return (NULL);
	}
	return (p.res);
}
