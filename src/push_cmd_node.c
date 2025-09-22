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

// void add_cmd_node(t_cmd_list *cmd_list, char *cmd, char *args,
//                   char *input_redir, int input_type, char *output_redir,
//                   int output_type)
// {
//     t_cmd_node *new_node;

//     new_node = malloc(sizeof(t_cmd_node));
//     if (!new_node)
//         return;
//     new_node->cmd = ft_strdup(cmd);
//     new_node->args = ft_strdup(args);
//     new_node->input_redir = ft_strdup(input_redir);
//     new_node->input_type = input_type;
//     new_node->output_redir = ft_strdup(output_redir);
//     new_node->output_type = output_type;
//     new_node->prev = NULL;
//     new_node->next = NULL;
//     push_cmd_node(cmd_list, new_node);
// }