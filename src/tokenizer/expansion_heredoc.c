/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/01/14 15:00:00 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_escaped_char_heredoc(t_minishell *sh,
				const char *s, t_expansion *exp)
{
	if (s[exp->i + 1] == '$')
	{
		exp->result = handle_char(sh, '$', exp->result);
		exp->i += 2;
		return (exp->result);
	}
	if (s[exp->i + 1] == 'n')
	{
		exp->result = handle_char(sh, 'n', exp->result);
		exp->i += 2;
		return (exp->result);
	}
	exp->result = handle_char(sh, s[exp->i + 1], exp->result);
	exp->i += 2;
	return (exp->result);
}

static char	*process_heredoc_chars(t_minishell *sh,
	const char *s, t_expansion *exp)
{
	if (s[exp->i] == '"' && !exp->in_single)
	{
		handle_quotes(s[exp->i], exp);
	}
	else if (s[exp->i] == '\\' && s[exp->i + 1] && !exp->in_single)
	{
		exp->result = handle_escaped_char_heredoc(sh, s, exp);
	}
	else if (s[exp->i] == '$' && !exp->in_single)
	{
		exp->result = process_dollar(sh, s, exp);
	}
	else
	{
		exp->result = handle_char(sh, s[exp->i], exp->result);
		exp->i++;
	}
	return (exp->result);
}

static char	*process_heredoc_expansion(t_minishell *sh, const char *s)
{
	t_expansion	exp;

	exp.i = 0;
	exp.in_single = 0;
	exp.in_double = 0;
	exp.result = gc_strdup(sh, "");
	if (!exp.result)
		return (NULL);
	while (s[exp.i])
	{
		exp.result = process_heredoc_chars(sh, s, &exp);
	}
	return (exp.result);
}

char	*expand_vars_heredoc(t_minishell *sh, const char *s)
{
	if (!sh || !s)
		return (NULL);
	return (process_heredoc_expansion(sh, s));
}
