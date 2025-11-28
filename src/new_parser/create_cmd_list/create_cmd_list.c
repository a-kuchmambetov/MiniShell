#include "create_cmd_list_utils.h"

static int set_errno(int code, int *errno)
{
	if (errno)
		*errno = code;
	return (code);
}

static int has_content(t_cmd_node *node)
{
	if (!node)
		return (0);
	return (node->cmd || node->args || node->input_redir || node->output_redir);
}

static size_t arg_count(char **args)
{
	size_t count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

static int append_arg(t_cmd_node *node, const char *value, int *errno)
{
	char **new_args;
	size_t count;
	size_t i;

	count = arg_count(node->args);
	new_args = ft_calloc(sizeof(char *), count + 2);
	if (!new_args)
		return (set_errno(1, errno));
	i = 0;
	while (i < count)
	{
		new_args[i] = node->args[i];
		i++;
	}
	new_args[count] = ft_strdup(value);
	if (!new_args[count])
		return (free(new_args), set_errno(1, errno));
	free(node->args);
	node->args = new_args;
	return (0);
}

static int handle_word(t_cmd_node *node, t_token_node *tkn, int *errno)
{
	if (!tkn->value || *(tkn->value) == '\0')
		return (0);
	if (!node->cmd)
	{
		node->cmd = ft_strdup(tkn->value);
		if (!node->cmd)
			return (*errno = 1);
		append_arg(node, node->cmd, errno);
		if (*errno)
			return (1);
		return (0);
	}
	return (append_arg(node, tkn->value, errno));
}

static int handle_input_redir(t_shell_data *dt, t_cmd_node *node,
							  t_token_node *tkn, int *errno)
{
	if (tkn->type == TOKEN_HEREDOC || tkn->type == TOKEN_HEREDOC_QUOTES)
	{
		my_free(node->input_redir);
		node->input_redir = NULL;
		node->input_redir_type = REDIR_HEREDOC;
		if (start_here_doc(dt->env_list, tkn->value, &node->input_redir))
			return (set_errno(1, errno));
	}
	else
	{
		if (node->failed_code != NO_FAIL)
			return (0);
		my_free(node->input_redir);
		node->input_redir = NULL;
		node->input_redir_type = REDIR_INPUT;
		node->input_redir = ft_strdup(tkn->value);
		if (!node->input_redir)
			return (set_errno(1, errno));
	}
	if (node->input_redir_type == REDIR_INPUT && check_file_open(node->input_redir) == 1)
	{
		node->failed_code = FAILED_IN;
		return (0);
	}
	return (0);
}

static int handle_output_redir(t_cmd_node *node, t_token_node *tkn, int *errno)
{
	if (node->failed_code != NO_FAIL)
		return (0);
	if (node->output_redir)
	{
		my_free(node->output_redir);
		node->output_redir = NULL;
	}
	if (tkn->type == TOKEN_APPEND)
		node->output_redir_type = REDIR_APPEND;
	else
		node->output_redir_type = REDIR_OUTPUT;
	node->output_redir = ft_strdup(tkn->value);
	if (!node->output_redir)
		return (set_errno(1, errno));
	if (node->output_redir && check_file_create(node->output_redir, node->output_redir_type) == 1)
	{
		node->failed_code = FAILED_OUT;
		return (0);
	}
	return (0);
}

static int process_token(t_shell_data *dt, t_token_node *tkn,
						 t_cmd_node **node, int *errno)
{
	if (!tkn)
		return (0);
	if (tkn->type == TOKEN_PIPE)
	{
		(*node)->is_pipe_out = 1;
		push_cmd_node(&dt->cmd_list, *node);
		*node = malloc_cmd_node(errno);
		if (*errno)
			return (1);
		(*node)->is_pipe_in = 1;
		return (0);
	}
	if (tkn->type == TOKEN_REDIR_IN || tkn->type == TOKEN_HEREDOC || tkn->type == TOKEN_HEREDOC_QUOTES)
		return (handle_input_redir(dt, *node, tkn, errno));
	if (tkn->type == TOKEN_REDIR_OUT || tkn->type == TOKEN_APPEND)
		return (handle_output_redir(*node, tkn, errno));
	return (handle_word(*node, tkn, errno));
}

static void build_commands(t_shell_data *dt, t_token_node *cur,
						   t_cmd_node **node, int *errno)
{
	while (cur)
	{
		if (process_token(dt, cur, node, errno) == 0)
			cur = cur->next;
		else
			break;
	}
	return;
}

int create_cmd_list(t_shell_data *dt, t_token_list *tkn_li, int *errno)
{
	t_token_node *cur;
	t_cmd_node *node;

	if (!dt || !tkn_li || !errno)
		return (1);
	*errno = 0;
	node = malloc_cmd_node(errno);
	if (*errno)
		return (1);
	cur = tkn_li->head;
	if (!cur)
		return (free_cmd_node(node), 1);
	build_commands(dt, cur, &node, errno);
	if (*errno)
		return (free_cmd_node(node), free_cmd_list(&dt->cmd_list), 1);
	if (has_content(node))
		push_cmd_node(&dt->cmd_list, node);
	else
		free_cmd_node(node);
	return (0);
}
