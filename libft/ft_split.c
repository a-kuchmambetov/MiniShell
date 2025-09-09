/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-29 09:12:36 by akuchmam          #+#    #+#             */
/*   Updated: 2025-04-29 09:12:36 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *str, const char c)
{
	size_t	count;

	count = 0;
	if (!str || *str == '\0' )
		return (count);
	while (*str == c)
		str++;
	while (*str)
	{
		if ((*str == c && *(str + 1) != c) || (*(str + 1) == '\0' && *str != c))
			count++;
		str++;
	}
	return (count);
}

static char	*add_word(const char *src, size_t len)
{
	char	*str;
	size_t	index;

	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	index = 0;
	while (index < len)
	{
		str[index] = src[index];
		index++;
	}
	str[len] = '\0';
	return (str);
}

static void	free_arr(char **str, size_t size)
{
	size_t	i;

	i = 0;
	if (!str)
		return ;
	while (i < size)
	{
		if (str[i])
			free(str[i]);
		i++;
	}
	free(str);
}

static char	**check_edge_case(const char *s, size_t word_count)
{
	char	**str;

	if (!s || word_count == 0 || *s == '\0')
		return (ft_calloc(sizeof(char *), 1));
	str = malloc(sizeof(char *) * (word_count + 1));
	if (!str)
		return (NULL);
	str[word_count] = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	const size_t	words = count_words(s, c);
	size_t			index_c[2];
	size_t			word_len;
	char			**str;

	str = check_edge_case(s, words);
	if (!s || *s == '\0')
		return (str);
	index_c[0] = 0;
	index_c[1] = 0;
	while (s[index_c[0]] == c && s[index_c[0]])
		index_c[0]++;
	while (s[index_c[0]] != '\0')
	{
		word_len = 0;
		while (s[index_c[0] + word_len] != c && s[index_c[0] + word_len])
			word_len++;
		str[index_c[1]++] = add_word(s + index_c[0], word_len);
		if (!(str[index_c[1] - 1]))
			return (free_arr(str, words + 1), NULL);
		while (s[index_c[0] + word_len] == c && s[index_c[0] + word_len])
			word_len++;
		index_c[0] += word_len;
	}
	return (str);
}
