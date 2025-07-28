/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 21:36:54 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/23 21:07:34 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*process_escaped_character(t_minishell *sh, const char *s,
					t_expansion *exp, char escaped_char)
{
	(void)s;
	exp->result = handle_char(sh, escaped_char, exp->result);
	exp->i += 2;
	return (exp->result);
}

static char	*handle_escaped_char(t_minishell *sh,
				const char *s, t_expansion *exp)
{
	if (s[exp->i + 1] == '$')
		return (process_escaped_character(sh, s, exp, '$'));
	if (s[exp->i + 1] == 'n')
		return (process_escaped_character(sh, s, exp, 'n'));
	if (s[exp->i + 1] == 't')
		return (process_escaped_character(sh, s, exp, 't'));
	if (s[exp->i + 1] == 'r')
		return (process_escaped_character(sh, s, exp, 'r'));
	if (s[exp->i + 1] == '\\')
		return (process_escaped_character(sh, s, exp, '\\'));
	return (process_escaped_character(sh, s, exp, s[exp->i + 1]));
}

static char	*process_characters(t_minishell *sh,
	const char *s, t_expansion *exp)
{
	if ((s[exp->i] == '\'' && !exp->in_double)
		|| (s[exp->i] == '"' && !exp->in_single))
	{
		handle_quotes(s[exp->i], exp);
	}
	else if (s[exp->i] == '\\' && s[exp->i + 1] && !exp->in_single)
	{
		exp->result = handle_escaped_char(sh, s, exp);
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

static char	*process_expansion(t_minishell *sh, const char *s)
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
		exp.result = process_characters(sh, s, &exp);
	}
	return (exp.result);
}

char	*expand_vars(t_minishell *sh, const char *s)
{
	if (!sh || !s)
		return (NULL);
	return (process_expansion(sh, s));
}
