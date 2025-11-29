/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:48:30 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:48:33 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "split_input.h"

static int	quote_parser(const char *s, t_split_data *dt, const char quote_char)
{
	dt->in_quote = YES_QUOTE;
	if (dt->i - dt->l > 0)
	{
		if (handle_add_row(s, dt))
			return (1);
		dt->l = dt->i;
	}
	dt->i++;
	while (s[dt->i] && dt->in_quote != NO_QUOTE)
	{
		if (s[dt->i] == quote_char)
			dt->in_quote = NO_QUOTE;
		dt->i++;
	}
	if (s[dt->i] == '\0' && dt->in_quote != NO_QUOTE)
		return (ft_print_err("Error: Unclosed quote\n"), free_str_arr(dt->arr),
			1);
	if (handle_add_row(s, dt))
		return (1);
	dt->l = dt->i;
	return (0);
}

static int	regular_parser(const char *s, t_split_data *dt)
{
	int	delim_len;

	delim_len = 0;
	if (dt->i > 0 && s[dt->i - 1] == ' ' && s[dt->i] == ' ')
	{
		skip_spaces(s, dt);
		return (0);
	}
	delim_len = is_delim(s + dt->i);
	if (delim_len > 0)
	{
		if (process_delimiter(s, dt, delim_len))
			return (1);
	}
	else
		dt->i++;
	return (0);
}

static void	select_parser(const char *s, t_split_data *dt, int *errno)
{
	if ((s[dt->i] == '\'' || s[dt->i] == '\"'))
	{
		*errno = quote_parser(s, dt, s[dt->i]);
		if (*errno)
			return ;
	}
	else
	{
		*errno = regular_parser(s, dt);
		if (*errno)
			return ;
	}
	*errno = 0;
}

char	**split_input(const char *s, int *errno)
{
	t_split_data	dt;

	dt = (t_split_data){0};
	while (s[dt.i])
	{
		select_parser(s, &dt, errno);
		if (*errno)
			return (NULL);
	}
	if (dt.i - dt.l > 0)
	{
		*errno = handle_add_row(s, &dt);
		if (*errno)
			return (NULL);
	}
	*errno = 0;
	return (dt.arr);
}
