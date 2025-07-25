/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_whitespace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 00:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:16:15 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int	count_words(char *str)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (!is_whitespace(*str))
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		str++;
	}
	return (count);
}

char	*extract_word(char *str, int *start)
{
	int		end;
	int		len;
	int		i;
	char	*word;

	while (str[*start] && is_whitespace(str[*start]))
		(*start)++;
	if (!str[*start])
		return (NULL);
	end = *start;
	while (str[end] && !is_whitespace(str[end]))
		end++;
	len = end - *start;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[*start + i];
		i++;
	}
	word[len] = '\0';
	*start = end;
	return (word);
}

char	**split_whitespace(char *str)
{
	char	**words;
	int		word_count;
	int		start;
	int		i;

	if (!str)
		return (NULL);
	word_count = count_words(str);
	words = malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	start = 0;
	i = 0;
	while (i < word_count)
	{
		words[i] = extract_word(str, &start);
		if (!words[i])
			return (NULL);
		i++;
	}
	words[i] = NULL;
	return (words);
}
