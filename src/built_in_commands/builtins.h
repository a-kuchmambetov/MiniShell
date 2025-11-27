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

char	*get_env_value(char **envp, const char *name);
int		is_builtin(char *cmd);
int		exec_builtin(t_shell_data *data, char **args);
int		add_or_update_env(t_shell_data *data, const char *arg);
int		sync_envp(t_shell_data *data);
int		is_valid_identifier(const char *str);
int		builtin_echo(char **argv);
int		builtin_cd(t_shell_data *data, char **args);
int		builtin_pwd(t_shell_data *data, char **args);
int		builtin_export(t_shell_data *data, char **args);
int		builtin_unset(t_shell_data *data, char **args);
int		builtin_env(t_shell_data *data);
int		builtin_exit(t_shell_data *data, char **args);
int		change_dir_update(t_shell_data *data, char *pa, char *old, char **args);
long	ft_atoi_long(const char *str, bool *overflow);
char	*build_final_pair(char *name, char *clean);
int		cleanup_return(char *key, char *value, int res);
void	print_export_list(t_env_node *node);
void	clean_process_assigment(char *name, char *val, char *final);
int		error_not_valid_identifier(char *arg, char *name);

#endif
