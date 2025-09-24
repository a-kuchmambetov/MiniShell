#include "builtins.h"

static int append_char (t_parser *p, char c)
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

static void expand_var(t_parser *p)
{
    int start;
    int len;
    char var[128];
    char *val;
    int j;

    p->i++;
    start = p->i;
	while ((p->s[p->i] >= 'a' && p->s[p->i] <= 'z') ||
		   (p->s[p->i] >= 'A' && p->s[p->i] <= 'Z') ||
		   (p->s[p->i] >= '0' && p->s[p->i] <= '9') ||
		   p->s[p->i] == '_')
		p->i++;
    len = p->i - start;
    if (len >= 128)
        len = 127;
    ft_strlcpy(var, p->s + start, len + 1);
    var[len] = '\0';
    val = NULL;
    j = 0;
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

static void parse_single_quotes(t_parser *p)
{
    char    quote;

    quote = p->s[p->i++];
    while (p->s[p->i] && p->s[p->i] != quote)
        append_char(p, p->s[p->i++]);
    if (p->s[p->i] == quote)
        p->i++;    
}

static void parse_double_or_plain(t_parser *p)
{
    char quote;
    if (p->s[p->i] == '"')
    {
        quote = p->s[p->i++];
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
    else if(p->s[p->i] == '$')
        expand_var(p);
    else
        append_char(p, p->s[p->i++]);
}

static char *parse_arg(const char *s, char **envp)
{
    t_parser    p;

    if (!s)
    return NULL;
    p.bufsize = 64;
    p.res = malloc(p.bufsize);
    if (!p.res)
        return (NULL);
    p.s = s;
    p.envp = envp;
    p.pos = 0;
    p.i = 0;
    while (p.s[p.i])
    {
        if (p.s[p.i] == '\'')
            parse_single_quotes(&p);
        else
            parse_double_or_plain(&p);
    }
    if (!append_char(&p, '\0'))
    {
        free(p.res);
        return NULL;
    }
    return (p.res);
}
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

int builtin_echo(char **argv, char **envp)
{
    int i;
    int n_flag;
    char *arg;

    n_flag = 0;
    i = 1;
    while (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
    {
        if (!all_n(argv[i] + 1))
            break ;
        n_flag = 1;
        i++;
    }
    while (argv[i])
    {
        arg = parse_arg(argv[i], envp);
        if(!arg)
            return (1);
        write (1, arg, ft_strlen(arg));
        free(arg);
        if(argv[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!n_flag)
        write (1, "\n", 1);
    return (0);
}