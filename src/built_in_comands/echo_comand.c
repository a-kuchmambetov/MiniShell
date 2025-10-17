#include "builtins.h"
#include "builtins.h"

/**
 * @brief Appends a character to parser buffer, reallocating if needed.
 *
 * @param p Pointer to parser structure.
 * @param c Character to append.
 * @return 1 on success, 0 on allocation failure.
 */
static int append_char(t_parser *p, char c)
{
    char *newbuf;
    int newsize;

    if (p->pos + 1 >= p->bufsize)
    {
        newsize = p->bufsize * 2;
        newbuf = malloc(newsize);
        if (!newbuf)
            return (0);
        ft_memcpy(newbuf, p->res, p->pos);
        free(p->res);
        p->res = newbuf;
        p->bufsize = newsize;
    }
    p->res[p->pos++] = c;
    return (1);
}

/**
 * @brief Expands environment variable in a string starting with '$'.
 *
 * @param p Pointer to parser structure.
 */
static void expand_var(t_parser *p)
{
    int start = ++p->i;
    char var[128];
    char *val = NULL;
    int j = 0;
    int len;

    while ((p->s[p->i] >= 'a' && p->s[p->i] <= 'z') ||
           (p->s[p->i] >= 'A' && p->s[p->i] <= 'Z') ||
           (p->s[p->i] >= '0' && p->s[p->i] <= '9') || p->s[p->i] == '_')
        p->i++;
    len = p->i - start;
    if (len >= 128)
        len = 127;
    ft_strlcpy(var, p->s + start, len + 1);

    while (p->envp && p->envp[j])
    {
        if (!ft_strncmp(p->envp[j], var, len) && p->envp[j][len] == '=')
        {
            val = p->envp[j] + len + 1;
            break;
        }
        j++;
    }
    while (val && *val)
        append_char(p, *val++);
}

/**
 * @brief Parses string enclosed in single quotes.
 *
 * @param p Pointer to parser structure.
 */
static void parse_single_quotes(t_parser *p)
{
    char quote = p->s[p->i++];
    while (p->s[p->i] && p->s[p->i] != quote)
        append_char(p, p->s[p->i++]);
    if (p->s[p->i] == quote)
        p->i++;
}

/**
 * @brief Parses double quotes, variable expansions, or plain text.
 *
 * @param p Pointer to parser structure.
 */
static void parse_double_or_plain(t_parser *p)
{
    if (p->s[p->i] == '"')
    {
        char quote = p->s[p->i++];
        while (p->s[p->i] && p->s[p->i] != quote)
        {
            if (p->s[p->i] == '$')
                expand_var(p);
            else
                append_char(p, p->s[p->i++]);
        }
        if (p->s[p->i] == quote)
            p->i++;
    }
    else if (p->s[p->i] == '$')
        expand_var(p);
    else
        append_char(p, p->s[p->i++]);
}

/**
 * @brief Parses a single argument string, expanding quotes and variables.
 *
 * @param s Input string.
 * @param envp Environment variables.
 * @return Allocated parsed string, or NULL on failure.
 */
static char *parse_arg(const char *s, char **envp)
{
    t_parser p;

    if (!s)
        return NULL;
    p.bufsize = 64;
    p.res = malloc(p.bufsize);
    if (!p.res)
        return NULL;
    p.s = s;
    p.envp = envp;
    p.pos = 0;
    p.i = 0;

    while (p.s[p.i])
        (p.s[p.i] == '\'') ? parse_single_quotes(&p) : parse_double_or_plain(&p);

    if (!append_char(&p, '\0'))
    {
        free(p.res);
        return NULL;
    }
    return p.res;
}

/**
 * @brief Checks if string consists only of 'n' characters.
 *
 * @param str Input string.
 * @return 1 if only 'n', 0 otherwise.
 */
int all_n(char *str)
{
    int j = 0;
    while (str[j])
    {
        if (str[j] != 'n')
            return 0;
        j++;
    }
    return 1;
}

/**
 * @brief Implements echo builtin with '-n' option and variable expansion.
 *
 * @param argv Command arguments.
 * @param envp Environment variables.
 * @return 0 on success, 1 on failure.
 */
int builtin_echo(char **argv, char **envp)
{
    int i = 1, n_flag = 0;
    char *arg;

    while (argv[i] && !ft_strncmp(argv[i], "-n", 2))
    {
        if (!all_n(argv[i] + 1))
            break;
        n_flag = 1;
        i++;
    }

    while (argv[i])
    {
        arg = parse_arg(argv[i], envp);
        if (!arg)
            return 1;
        write(1, arg, ft_strlen(arg));
        free(arg);
        if (argv[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!n_flag)
        write(1, "\n", 1);
    return 0;
}
