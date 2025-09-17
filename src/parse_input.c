#include "../main.h"

void push_cmd_node(t_cmd_list *cmd_list, t_cmd_node *new_node)
{
    t_cmd_node *current;

    if (!cmd_list || !new_node)
        return;
    current = cmd_list->first;
    if (!current)
    {
        cmd_list->first = new_node;
        cmd_list->len = 1;
        return;
    }
    while (current->next)
        current = current->next;
    new_node->prev = current;
    current->next = new_node;
    cmd_list->len++;
}

void add_cmd_node(t_cmd_list *cmd_list, char *cmd, char *args,
                  char *input_redir, int input_type, char *output_redir, 
                  int output_type)
{
    t_cmd_node *new_node;

    new_node = malloc(sizeof(t_cmd_node));
    if (!new_node)
        return;
    new_node->cmd = ft_strdup(cmd);
    new_node->args = ft_strdup(args);
    new_node->input_redir = ft_strdup(input_redir);
    new_node->input_type = input_type;
    new_node->output_redir = ft_strdup(output_redir);
    new_node->output_type = output_type;
    new_node->prev = NULL;
    new_node->next = NULL;
    push_cmd_node(cmd_list, new_node);
}

char *ft_strdup_size(const char *src, int len)
{
    char *dest;

    dest = ft_calloc(sizeof(char), len + 1);
    if (!dest)
        return (NULL);
    ft_strlcpy(dest, src, len + 1);
    return (dest);
}

void create_cmd_node(t_cmd_list *cmd_list, char *str, char *input)
{
    const char **parsed = ft_split(str, ' ');

    if (!parsed)
        return;
    if (ft_strcmp(input, "<") == 0)
        add_cmd_node(cmd_list, parsed[0], NULL, parsed[1], 1, NULL, -1);
    free_split(parsed);
}

int parse_input(char *input, t_cmd_list *cmd_list)
{
    int left;
    int i;
    int delim_len;

    left = 0;
    i = 0;
    while (input[i])
    {
        delim_len = is_delimiter(&input[i]);
        if (delim_len > 0)
        {
            // Handle the delimiter
            if (delim_len == 1)
                create_cmd_node(cmd_list, ft_strdup_size(input + left, i - left + 1), input[i]);
            else if (delim_len == 2)
            {
                // Handle double character delimiter
            }
            i += delim_len;
            left = i + 1;
        }
        i++;
    }

    return (0);
}

int is_delimiter(char *str)
{
    if (ft_strcmp(str, "<<") == 0 || ft_strcmp(str, ">>") == 0)
        return (2);
    if (ft_strcmp(str, "|") == 0 || ft_strcmp(str, ">") == 0 || ft_strcmp(str, "<") == 0)
        return (1);
    return (0);
}