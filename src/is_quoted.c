#include "../main.h"

int	is_quoted(char *str)
{
	if (!str)
		return (0);
	if ((str[0] == '"' || str[0] == '\'') && str[0] == str[ft_strlen(str) - 1])
		return (1);
	return (0);
}