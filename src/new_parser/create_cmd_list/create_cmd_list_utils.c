#include "create_cmd_list_utils.h"

int set_errno(int code, int *errno)
{
	if (errno)
		*errno = code;
	return (code);
}

int has_content(t_cmd_node *node)
{
	if (!node)
		return (0);
	return (node->cmd || node->args || node->input_redir || node->output_redir);
}

size_t arg_count(char **args)
{
	size_t count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

int append_arg(t_cmd_node *node, const char *value, int *errno)
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
