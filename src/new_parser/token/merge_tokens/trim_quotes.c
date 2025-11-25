#include "new_parser/parser.h"

char *trim_quotes(char *str)
{
    int i;

    if (!str)
        return (NULL);
    i = 0;
    if ((str[0] == '\"' || str[0] == '\'') && str[0] == str[ft_strlen(str) - 1])
    {
        while (str[i])
        {
            str[i] = str[i + 1];
            i++;
        }
        str[i - 1] = '\0';
    }
    return (str);
}

char *trim_space_before(char *str)
{
    int i;
    int j;

    if (!str)
        return (NULL);
    i = 0;
    while (str[i] == ' ')
        i++;
    if (i == 0)
        return (str);
    j = 0;
    while (str[i])
    {
        str[j] = str[i];
        i++;
        j++;
    }
    str[j] = '\0';
    return (str);
}

char *trim_space_after(char *str)
{
    int i;
    int j;

    if (!str)
        return (NULL);
    i = ft_strlen(str) - 1;
    while (str[i] == ' ')
        i--;
    if (i == 0)
        return (str);
    j = 0;
    while (str[i])
    {
        str[j] = str[i];
        i++;
        j++;
    }
    str[j] = '\0';
    return (str);
}