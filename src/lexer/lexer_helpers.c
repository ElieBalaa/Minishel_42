/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:12:56 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 16:16:55 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	advance_word(const char **ptr)
{
	const char	*p;

	p = *ptr;
	while (*p && !is_space(*p) && !is_operator(*p))
	{
		if (*p == '\'' || *p == '"')
			p = skip_quotes(p);
		else if (*p == '\\' && p[1])
			p += 2;
		else
			p++;
	}
	*ptr = p;
}

void	process_operator(t_token **lst, t_minishell *sh, const char **p)
{
	int	len;

	if (**p == '<' && *(*p + 1) == '<')
	{
		add_tok(lst, sh, *p, 2);
		*p += 2;
		return ;
	}
	if (**p == '|' && *(*p + 1) == '|')
	{
		add_tok(lst, sh, *p, 2);
		*p += 2;
		return ;
	}
	if (**p == '&' && *(*p + 1) == '&')
	{
		add_tok(lst, sh, *p, 2);
		*p += 2;
		return ;
	}
	len = op_len(*p);
	add_tok(lst, sh, *p, len);
	*p += len;
}

void	process_word(t_token **lst, t_minishell *sh, const char **p)
{
	const char	*start;
	int			len;

	start = *p;
	advance_word(p);
	len = (int)(*p - start);
	add_tok(lst, sh, start, len);
}
