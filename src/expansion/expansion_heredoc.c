/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/27 18:44:41 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*append_str(t_minishell *sh, char *res, const char *add)
{
	size_t	j;

	j = 0;
	while (add[j])
	{
		res = handle_char(sh, add[j], res);
		j++;
	}
	return (res);
}

static char	*handle_escaped_dollar(t_minishell *sh,
	const char *s, size_t *i, char *res)
{
	(void)s;
	res = handle_char(sh, '$', res);
	*i += 2;
	return (res);
}

static char	*process_heredoc_var(t_minishell *sh,
	const char *s, size_t *i, char *res)
{
	size_t	start;
	char	*var;
	char	*val;

	start = ++(*i);
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	if (*i > start)
	{
		var = ft_substr(s, start, *i - start);
		val = get_env_var(sh, var);
		free(var);
		if (val)
			res = append_str(sh, res, val);
	}
	return (res);
}

static char	*process_heredoc_char(t_minishell *sh,
	const char *s, size_t *i, char *res)
{
	if (s[*i] == '\\' && s[*i + 1] == '$')
		return (handle_escaped_dollar(sh, s, i, res));
	else if (s[*i] == '$')
		return (process_heredoc_var(sh, s, i, res));
	else
	{
		res = handle_char(sh, s[*i], res);
		(*i)++;
		return (res);
	}
}

char	*expand_vars_heredoc(t_minishell *sh, const char *s)
{
	char	*res;
	size_t	i;

	if (!sh || !s)
		return (NULL);
	res = gc_strdup(sh, "");
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
		res = process_heredoc_char(sh, s, &i, res);
	return (res);
}
