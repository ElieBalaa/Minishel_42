/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:40:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 16:40:00 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*create_split_token(t_minishell *sh, const char *text)
{
	t_token	*token;

	token = gc_malloc(sh, sizeof(*token));
	if (!token)
		return (NULL);
	token->text = gc_strdup(sh, text);
	if (!token->text)
		return (NULL);
	token->quoted = 0;
	token->next = NULL;
	return (token);
}

static t_token	*create_token_chain(t_minishell *sh, char **words,
							t_token *next)
{
	t_token	*first;
	t_token	*current;
	t_token	*new_token;
	int		i;

	first = create_split_token(sh, words[0]);
	if (!first)
		return (NULL);
	current = first;
	i = 1;
	while (words[i])
	{
		new_token = create_split_token(sh, words[i]);
		if (!new_token)
			return (first);
		current->next = new_token;
		current = new_token;
		i++;
	}
	current->next = next;
	return (first);
}

t_token	*split_unquoted_token(t_minishell *sh, t_token *token)
{
	char	**words;
	t_token	*result;

	if (token->quoted || !ft_strchr(token->text, ' '))
		return (token);
	words = split_whitespace(token->text);
	if (!words || !words[0])
	{
		ft_free_split(words);
		return (token);
	}
	result = create_token_chain(sh, words, token->next);
	ft_free_split(words);
	if (!result)
		return (token);
	return (result);
}

t_token	*apply_word_splitting(t_minishell *sh, t_token *tokens)
{
	t_token	*result;
	t_token	*current;
	t_token	*next;
	t_token	*split_result;

	if (!tokens)
		return (NULL);
	result = split_unquoted_token(sh, tokens);
	current = result;
	while (current && current->next)
	{
		next = current->next;
		split_result = split_unquoted_token(sh, next);
		current->next = split_result;
		while (current->next != next && current->next)
			current = current->next;
		if (current->next == next)
			current = current->next;
	}
	return (result);
}
