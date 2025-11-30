/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 03:30:23 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/30 03:33:15 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// Project custom headers
# include "libft/libft.h"
# include "src/new_parser/parser.h"
# include "types.h"

extern volatile sig_atomic_t	g_signal_received;

// Configuration
# define HERE_DOC_DIR "./"
# define HERE_DOC_PREFIX "here_doc_"
# define PATH_MAX 4096
// ANSI color codes
# define COLOR_RESET "\033[0m"
# define COLOR_RED "\033[31m"
# define COLOR_GREEN "\033[32m"
# define COLOR_YELLOW "\033[33m"
# define COLOR_BLUE "\033[34m"
# define COLOR_MAGENTA "\033[35m"
# define COLOR_CYAN "\033[36m"
# define COLOR_WHITE "\033[37m"
# define COLOR_BOLD "\033[1m"
# define COLOR_DIM "\033[2m"

int		ft_print_err(const char *format, ...);

void	parse_envp(t_shell_data *data, char **envp);
void	parse_exec_folders(t_shell_data *data);

void	my_free(void *ptr);
void	free_cmd_node(t_cmd_node *node);
void	free_cmd_list(t_cmd_list *cmd_list);
void	free_env_list(t_env_list *env);
void	free_str_arr(char **paths);
void	free_shell_data(t_shell_data *data);

int		set_envp_from_env(t_shell_data *data);
int		init_shell_data(t_shell_data *data, char **envp);
int		update_last_exit_status(t_shell_data *dt, int last_cmd_code);

/// Built-in commands
int		is_builtin(char *cmd);
int		exec_builtin(t_shell_data *data, char **args);

char	*process_expansion(t_env_list env, char *input, int *errno,
			int *current_type);
int		start_here_doc(t_env_list env, const char *eof_word, char **filename);
int		delete_here_doc(const char *filename);
int		parse_input(t_shell_data *data, char *input);

#endif