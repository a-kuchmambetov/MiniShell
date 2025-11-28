#include "expand_tokens.h"

char *squash_spaces(char *src)
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