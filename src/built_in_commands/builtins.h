/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:19:39 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/23 12:25:08 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../../main.h"
# include <limits.h>
# include <stdbool.h>
# include <errno.h>

typedef struct s_parser
{
	const char	*s;
	char		*res;
	int			pos;
	int			bufsize;
	char		**envp;
	int			i;
}	t_parser;

char	*get_env_value(char **envp, const char *name);
char	*parse_arg(const char *s, char **envp);
int		is_builtin(char *cmd);
int		exec_builtin(t_shell_data *data, char **args);
int		add_or_update_env(t_shell_data *data, const char *arg);
int		sync_envp(t_shell_data *data);
int		is_valid_identifier(const char *str);
int		builtin_echo(char **argv, char **envp);
int		builtin_cd(t_shell_data *data, char **args);
int		builtin_pwd(t_shell_data *data, char **args);
int		builtin_export(t_shell_data *data, char **args);
int		builtin_unset(t_shell_data *data, char **args);
int		builtin_env(t_shell_data *data, char **args);
int		builtin_exit(t_shell_data *data, char **args);
int		change_dir_update(t_shell_data *data, char *pa, char *old, char **args);
long	ft_atoi_long(const char *str, bool *overflow);
char	*strip_outer_quotes(const char *s);
char	get_opening_quote(const char *s);
void	join_quoted_parts(char **res, char **args, int *i, char quote);
char	*collect_value_after_equal(char **args, int *i);
char	*build_final_pair(char *name, char *clean);

#endif
