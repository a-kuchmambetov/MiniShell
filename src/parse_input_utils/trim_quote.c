#include "parse_input_utils.h"

char *trim_quote(const char *str)
{
    char *trimmed;

    if (!str)
        return (NULL);
    trimmed = ft_strtrim(str, (const char[]){str[0], '\0'});
    if (!trimmed)
        return (NULL);
    return (trimmed);
}