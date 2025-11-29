/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoroka <vmoroka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:52:37 by vmoroka           #+#    #+#             */
/*   Updated: 2025/11/29 12:52:39 by vmoroka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../../main.h"

typedef enum e_token_type
{
	TOKEN_HEREDOC = 0,
	TOKEN_REDIR_IN = 1,
	TOKEN_REDIR_OUT = 2,
	TOKEN_APPEND = 3,
	TOKEN_PIPE = 4,
	TOKEN_TEXT = 5,
	TOKEN_EXPANSION = 6,
	TOKEN_AMBIGUOUS = 7,
	TOKEN_HEREDOC_PART = 8,
	TOKEN_HEREDOC_QUOTES = 9
}						t_token_type;

typedef struct s_token_node
{
	char				*value;
	t_token_type		type;
	int					is_space_after;
	struct s_token_node	*next;
}						t_token_node;

typedef struct s_token_list
{
	t_token_node		*head;
	t_token_node		*tail;
}						t_token_list;

void					free_token_list(t_token_list *tkn_li);
void					free_token_node(t_token_node *node);

char					*trim_space_both(char *str);

char					**split_input(const char *s, int *errno);
t_token_list			*create_token_list(char **arr, int *errno);
void					expand_tokens(t_token_list *tkn_li, t_shell_data *data,
							int *errno);
char					*process_expansion(t_env_list env, char *input,
							int *errno, int *current_type);
void					split_expansion(t_token_list tkn_li, int *errno);
void					merge_tokens(t_token_list tkn_li, int *errno);
int						create_cmd_list(t_shell_data *dt, t_token_list *tkn_li,
							int *errno);

#endif