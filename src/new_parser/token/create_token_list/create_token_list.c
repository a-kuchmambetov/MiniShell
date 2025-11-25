#include "create_token_list.h"

static void add_tkn_to_li(t_token_node *tkn, t_token_list *tkn_li)
{
    if ((tkn_li->tail->type == TOKEN_HEREDOC || tkn_li->tail->type == TOKEN_HEREDOC_PART) && tkn_li->tail->is_space_after == 0 && tkn->type == TOKEN_TEXT)
        tkn->type = TOKEN_HEREDOC_PART;
    if (!tkn_li->head)
        tkn_li->head = tkn;
    else
        tkn_li->tail->next = tkn;
    tkn_li->tail = tkn;
}

t_token_list *create_token_list(char **arr, int *errno)
{
    t_token_list *tkn_li;
    t_token_node *tkn;
    int i;

    i = 0;
    tkn_li = malloc(sizeof(t_token_list));
    if (!tkn_li)
    {
        *errno = 1;
        return (NULL);
    }
    tkn_li->head = NULL;
    tkn_li->tail = NULL;
    while (arr[i])
    {
        tkn = set_tkn(arr, &i, errno);
        if (*errno)
        {
            free_token_list(tkn_li);
            return (NULL);
        }
        add_tkn_to_li(tkn, tkn_li);
        i++;
    }
    return (tkn_li);
}
