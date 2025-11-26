/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unadoroz <unadoroz@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:45:00 by unadoroz          #+#    #+#             */
/*   Updated: 2025/10/29 19:45:00 by unadoroz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/* Build "KEY=VALUE" string */
char *build_final_pair(char *name, char *clean)
{
	char *pair;
	char *final;

	pair = ft_strjoin(name, "=");
	if (!pair)
		return (NULL);
	final = ft_strjoin(pair, clean);
	my_free(pair);
	return (final);
}
