/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:03:33 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/27 18:36:12 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	cleanup_partial_array(char **arr, size_t i)
{
	while (i--)
		free(arr[i]);
	free(arr);
}

char	**tok_to_array(t_minishell *sh, t_token *lst)
{
	size_t	count;
	size_t	i;
	char	**arr;

	if (!sh || !lst)
		return (NULL);
	count = ft_tokensize(lst);
	arr = gc_malloc(sh, sizeof(*arr) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (lst)
	{
		arr[i] = gc_strdup(sh, lst->text);
		if (!arr[i])
		{
			cleanup_partial_array(arr, i);
			return (NULL);
		}
		++i;
		lst = lst->next;
	}
	arr[i] = NULL;
	return (arr);
}

t_ast	*parse_line(const char *line, t_minishell *sh)
{
	t_token	*tok;
	t_token	*split_tok;
	char	**words;

	tok = tokenize(line, sh);
	if (!tok)
		return (NULL);
	split_tok = apply_word_splitting(sh, tok);
	words = tok_to_array(sh, split_tok);
	if (!words)
		return (NULL);
	return (parse_pipeline(words, sh));
}

const char	*skip_quotes(const char *p)
{
	char	q;

	q = *p++;
	while (*p && *p != q)
	{
		if (q == '"' && *p == '\\' && p[1])
			p += 2;
		else
			p++;
	}
	if (*p == q)
		p++;
	return (p);
}
