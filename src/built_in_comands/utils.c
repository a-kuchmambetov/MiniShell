#include "builtins.h"

/**
 * @brief Checks if a string is a valid shell identifier.
 *
 * A valid identifier starts with a letter or underscore ('_') and
 * may contain letters, digits, or underscores. Stops checking at '='.
 *
 * @param str The string to validate.
 * @return 1 if the identifier is valid, 0 otherwise.
 */
int is_valid_identifier(const char *str)
{
    int i;

    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    i = 0;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}
