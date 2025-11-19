#include "create_token_list.h"

static t_token_type get_tkn_type(char **arr, int i)
{
    if (ft_strncmp(arr[i], "|", 2) == 0)
        return (PIPE);
    if (ft_strncmp(arr[i], "<", 2) == 0)
        return (REDIR_IN);
    if (ft_strncmp(arr[i], ">", 2) == 0)
        return (REDIR_OUT);
    if (ft_strncmp(arr[i], ">>", 3) == 0)
        return (APPEND);
    if (ft_strncmp(arr[i], "<<", 3) == 0)
        return (HEREDOC);
    return (TEXT);
}

static char *get_tkn_val(char **arr, int *i, t_token_node tkn, int *ernno)
{
    char *copy;

    copy = NULL;
    if (tkn.type != PIPE && tkn.type != TEXT)
    {
        if (ft_strncmp(arr[*i + 1], " ", 2) != 0 && !is_delim(arr[*i + 1]))
        {
            *i += 1;
            copy = ft_strdup(arr[*i]);
        }
        if (ft_strncmp(arr[*i + 1], " ", 2) == 0 && !is_delim(arr[*i + 2]))
        {
            *i += 2;
            copy = ft_strdup(arr[*i]);
        }
        if (!copy)
            ft_print_error("wrong file name");
    }
    else
        copy = ft_strdup(arr[*i]);
    if (!copy)
        *ernno = 1;
    return (copy);
}

static int is_space_after(char **arr, int *i)
{
    if (arr[*i + 1] && ft_strncmp(arr[*i + 1], " ", 2) == 0)
    {
        *i += 1;
        return (1);
    }
    return (0);
}

t_token_node *set_tkn(char **arr, int *i, int *errno)
{
    t_token_node *tkn;

    tkn = malloc(sizeof(t_token_node));
    if (!tkn)
    {
        *errno = 1;
        return (NULL);
    }
    tkn->type = get_tkn_type(arr, *i);
    tkn->value = get_tkn_val(arr, i, *tkn, errno);
    if (*errno)
    {
        *errno = 1;
        my_free(tkn);
        return (NULL);
    }
    tkn->is_space_after = is_space_after(arr, i);
    tkn->next = NULL;
    return (tkn);
}
