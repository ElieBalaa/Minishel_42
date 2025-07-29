/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:18:34 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 16:17:21 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

static int	has_dollar_sign(const char *start, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (start[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

static int	check_heredoc_delimiter(t_token **lst)
{
	t_token	*last;

	if (!*lst)
		return (0);
	last = tok_last(*lst);
	if (last && ft_strcmp(last->text, "<<") == 0)
		return (1);
	return (0);
}

void	add_tok(t_token **lst, t_minishell *sh, const char *start, int len)
{
	t_token	*node;
	char	*text;
	int		is_heredoc_delim;
	int		was_quoted;
	int		has_expansion;

	is_heredoc_delim = check_heredoc_delimiter(lst);
	was_quoted = (len >= 2 && ((start[0] == '\'' && start[len - 1] == '\'')
				|| (start[0] == '"' && start[len - 1] == '"')));
	has_expansion = (!was_quoted && has_dollar_sign(start, len));
	text = process_token_content(sh, start, len, is_heredoc_delim);
	if (!text || (!*text && !was_quoted))
		return ;
	node = gc_malloc(sh, sizeof(*node));
	node->text = text;
	node->quoted = was_quoted || !has_expansion;
	node->next = NULL;
	if (!*lst)
		*lst = node;
	else
		tok_last(*lst)->next = node;
}

t_token	*tokenize(const char *line, t_minishell *sh)
{
	t_token		*lst;
	const char	*p;

	if (!line)
		return (NULL);
	lst = NULL;
	p = line;
	while (*p)
	{
		if (is_space(*p))
			p++;
		else if (is_operator(*p))
			process_operator(&lst, sh, &p);
		else
			process_word(&lst, sh, &p);
	}
	return (lst);
}

char	*process_quoted_delimiter(t_minishell *sh, const char *start, int len)
{
	char	*clean;

	clean = gc_strndup(sh, start + 1, len - 2);
	return (gc_strjoin(sh, "'", clean));
}
