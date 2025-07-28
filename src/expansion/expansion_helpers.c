/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 20:59:35 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:17:38 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes(char c, t_expansion *exp)
{
	if (c == '\'' && !exp->in_double)
		exp->in_single ^= 1;
	else if (c == '"' && !exp->in_single)
		exp->in_double ^= 1;
	exp->i++;
}

char	*handle_dollar_quote(t_minishell *sh,
	const char *s, t_expansion *exp)
{
	exp->i += 2;
	while (s[exp->i] && s[exp->i] != '"')
	{
		exp->result = handle_char(sh, s[exp->i], exp->result);
		exp->i++;
	}
	if (s[exp->i] == '"')
		exp->i++;
	return (exp->result);
}

char	*handle_ansi_c_quote(t_minishell *sh,
	const char *s, t_expansion *exp)
{
	size_t	start;
	size_t	end;
	char	*content;
	char	*tmp;

	exp->i += 2;
	start = exp->i;
	while (s[exp->i] && s[exp->i] != '\'')
		exp->i++;
	end = exp->i;
	if (s[exp->i] == '\'')
		exp->i++;
	content = gc_strndup(sh, s + start, end - start);
	tmp = gc_strjoin(sh, exp->result, content);
	return (tmp);
}

char	*process_dollar(t_minishell *sh, const char *s, t_expansion *exp)
{
	if (s[exp->i + 1] == '"' && !exp->in_single && !exp->in_double)
		return (handle_dollar_quote(sh, s, exp));
	if (s[exp->i + 1] == '\'' && !exp->in_single && !exp->in_double)
		return (handle_ansi_c_quote(sh, s, exp));
	return (handle_dollar(sh, s, &exp->i, exp->result));
}

char	*lookup_env_var(t_minishell *sh, const char *name, int len)
{
	int	i;

	i = 0;
	while (sh->env[i])
	{
		if (!ft_strncmp(sh->env[i], name, len)
			&& sh->env[i][len] == '=')
			return (gc_strdup(sh, sh->env[i] + len + 1));
		i++;
	}
	return (gc_strdup(sh, ""));
}
