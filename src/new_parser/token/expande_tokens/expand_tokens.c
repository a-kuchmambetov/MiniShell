#include "../../parser.h"

static char *squash_spaces(char *src)
{
    int i;
    int j;
    int in_space;

    i = -1;
    j = 0;
    in_space = 0;
    while (src[++i])
    {
        if (src[i] != ' ')
        {
            src[j++] = src[i];
            in_space = 0;
        }
        else if (!in_space)
        {
            src[j++] = ' ';
            in_space = 1;
        }
    }
    src[j] = '\0';
    return (src);
}

static int join_value(char **dest, const char *src, int *errno)
{
    char *new_str;

    if (!dest || !src)
    {
        *errno = 1; // is it right place for errno?
        return (1);
    }
    if (!*dest)
        *dest = ft_calloc(1, 1);
    new_str = ft_strjoin(*dest, src);
    if (!new_str)
    {
        *errno = 1; // is it right place for errno?
        return (1);
    }
    free(*dest);
    *dest = new_str;
    return (0);
}

static int is_ambig(char *src)
{
    int i;
    int words;
    int in_word;

    if (!src)
        return (0);
    i = 0;
    words = 0;
    in_word = 0;
    while (src[i])
    {
        if (src[i] == ' ' || src[i] == '\t' || src[i] == '\n')
            in_word = 0;
        else if (!in_word)
        {
            in_word = 1;
            words++;
            if (words > 1)
                return (1);
        }
        i++;
    }
    return (0);
}

static char *process_src(char *src, int in_quotes, int *check_token)
{
    if (!in_quotes)
    {
        if (*check_token == TOKEN_REDIR_IN || *check_token == TOKEN_REDIR_OUT || *check_token == TOKEN_APPEND)
            if (is_ambig(src))
            {
                *check_token = TOKEN_AMBIGUOUS; // prolly this var i needed to pas via & all up there, but you'll change it so..
                return (NULL);
            }
        return (squash_spaces(src));
    }
    return (src);
}

static char *get_val_by_key(t_env_list env, const char *name)
{
    t_env_node *current;
    size_t len;
    char *value;

    current = env.first;
    if (!name || !*name)
        return (NULL);
    len = ft_strlen(name);
    while (current)
    {
        if (!ft_strncmp(current->key, name, len))
        {
            value = ft_strdup(current->value);
            if (!value)
                return (NULL);
            return (value);
        }
        current = current->next;
    }
    return (NULL);
}

static int find_key_len(const char *str)
{
    int len;

    len = 0;
    if (!str || !*str)
        return (0);
    if (str[len] == '?')
        return (1);
    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;
    return (len);
}

static char *get_expanded_val(t_env_list env, char *input, int *i, int *errno)
{
    const int key_len = find_key_len(input + 1);
    char *key;
    char *env_value;

    if (key_len < 1)
    {
        env_value = ft_strdup("$");
        return (env_value);
    }
    key = ft_strndup(input + 1, key_len);
    if (!key)
    {
        *errno = 1;
        return (NULL);
    }
    env_value = get_val_by_key(env, key);
    if (!env_value)
    {
        env_value = ft_strdup("");
        if (!env_value)
            *errno = 1;
    }
    *i += key_len;
    return (my_free(key), env_value);
}

static char *get_next_src(t_env_list env, char *input, int *i, int *errno)
{
    if (input[*i] == '$')
        return (get_expanded_val(env, input + *i, i, errno));
    return (ft_strndup(input + *i, 1));
}

char *process_expansion(t_env_list env, char *input, int *errno, int *current_type)
{
    char *dest;
    char *src;
    int i;
    int in_quotes;

    i = 0;
    in_quotes = (input[0] == '"');
    src = NULL;
    dest = ft_strdup("");
    if (!dest)
        return (NULL);
    while (input[i])
    {
        src = get_next_src(env, input, &i, errno);
        if (!src || *errno)
            return (my_free(dest), NULL);
        src = process_src(src, in_quotes, current_type);
        if (!src)
            return (my_free(dest), NULL);
        if (join_value(&dest, src, errno))
            return (my_free(dest), NULL);
        my_free(src);
        i++;
    }
    return (dest);
}

static char *check_do_expansion(t_env_list env, char *input, int *errno, int *current_type)
{
    char *result;

    if (!input)
    {
        *errno = 1;
        return (NULL);
    }
    if (input[0] == '\'')
        return (NULL);
    result = process_expansion(env, input, errno, current_type);
    if (*current_type == TOKEN_TEXT && result)
        *current_type = TOKEN_EXPANSION;
    return (result);
}

static void update_value(t_token_node *cur, char *new_value, int *errno)
{
    my_free(cur->value);
    cur->value = ft_strdup(new_value);
    if (!cur->value)
        *errno = 1;
}

static void update_type(t_token_node *cur, int new_type)
{
    cur->type = new_type;
}

void expand_tokens(t_token_list *tkn_li, t_shell_data *data, int *errno)
{
    t_token_node *cur;
    int cur_t;
    char *src;

    cur = tkn_li->head;
    while (cur)
    {
        cur_t = cur->type;
        if (cur_t)
        {
            src = check_do_expansion(data->env_list, cur->value, errno, &cur_t);
            if (*errno)
                return (my_free(src), (void)NULL);
            if (src)
            {
                update_value(cur, src, errno);
                if (*errno)
                    return (my_free(src), (void)NULL);
                if (cur_t == TOKEN_AMBIGUOUS || cur_t == TOKEN_EXPANSION)
                    update_type(cur, cur_t);
            }
        }
        cur = cur->next;
    }
    return;
}
