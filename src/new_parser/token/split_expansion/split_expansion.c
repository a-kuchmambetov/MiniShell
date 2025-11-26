#include "split_expansion.h"

static int get_space_index(const char *str)
{
    int i;

    i = 1;
    while (str[i])
    {
        if (str[i] == ' ')
            return i;
        i++;
    }
    return 0;
}

static t_token_node *init_new_token_node(int *errno)
{
    t_token_node *new_tkn = malloc(sizeof(t_token_node));

    if (!new_tkn)
    {
        *errno = 1;
        return NULL;
    }
    new_tkn->value = NULL;
    new_tkn->type = TOKEN_TEXT;
    new_tkn->is_space_after = 0;
    new_tkn->next = NULL;
    return new_tkn;
}

static void process_split_node(t_token_node *cur, t_token_node *new_tkn, int space_index, int *errno)
{
    char *temp;

    temp = ft_strndup(cur->value, space_index);
    if (!temp)
    {
        *errno = 1;
        return;
    }
    new_tkn->value = ft_strdup(cur->value + space_index + 1);
    if (!new_tkn->value)
    {
        *errno = 1;
        my_free(temp);
        return;
    }
    my_free(cur->value);
    cur->type = TOKEN_TEXT;
    cur->value = trim_space_both(temp);
    new_tkn->is_space_after = cur->is_space_after;
    cur->is_space_after = 1;
    new_tkn->next = cur->next;
    cur->next = new_tkn;
    return;
}

static int is_quoted(char *str)
{
    if (!str)
        return (0);
    if ((str[0] == '"' || str[0] == '\'') && str[0] == str[ft_strlen(str) - 1])
        return (1);
    return (0);
}

void split_expansion(t_token_list tkn_li, int *errno)
{
    t_token_node *cur;
    t_token_node *new_tkn;
    int space_index;

    cur = tkn_li.head;
    while (cur)
    {
        if (cur->type == TOKEN_EXPANSION && !is_quoted(cur->value))
        {
            space_index = get_space_index(cur->value);
            if (space_index)
            {
                new_tkn = init_new_token_node(errno);
                if (*errno)
                    return;
                process_split_node(cur, new_tkn, space_index, errno);
                if (*errno)
                    return free_token_node(new_tkn);
            }
        }
        cur = cur->next;
    }
    return;
}
