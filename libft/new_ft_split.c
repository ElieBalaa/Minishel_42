/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ft_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:58:04 by oiskanda          #+#    #+#             */
/*   Updated: 2025/06/27 15:58:29 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_separator(char c, char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

int	word_count(char *str, char *charset)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (is_separator(*str, charset))
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

char	*allocate_word(char *start, char *end)
{
	int		len;
	char	*word;
	int		i;

	len = end - start;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

char	**ft_split_charset(char *str, char *charset)
{
	char	**words;
	int		i;
	int		word_index;
	char	*word_start;

	words = (char **)malloc(sizeof(char *) * (word_count(str, charset) + 1));
	if (!words)
		return (NULL);
	i = 0;
	word_index = 0;
	while (str[i])
	{
		if (!is_separator(str[i], charset))
		{
			word_start = str + i;
			while (str[i] && !is_separator(str[i], charset))
				i++;
			words[word_index++] = allocate_word(word_start, str + i);
		}
		else
			i++;
	}
	words[word_index] = NULL;
	return (words);
}
