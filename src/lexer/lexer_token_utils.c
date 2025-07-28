/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:45:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 16:45:00 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*process_heredoc_token(t_minishell *sh, const char *start, int len)
{
	char	*raw;
	char	*expd;

	raw = gc_strndup(sh, start, len);
	if (len >= 2 && ((start[0] == '\'' && start[len - 1] == '\'')
			|| (start[0] == '"' && start[len - 1] == '"')))
		return (process_quoted_delimiter(sh, start, len));
	expd = expand_vars(sh, raw);
	return (expd);
}

static char	*process_regular_token(t_minishell *sh, const char *start, int len)
{
	char	*raw;
	char	*expd;

	raw = gc_strndup(sh, start, len);
	expd = expand_vars(sh, raw);
	return (expd);
}

char	*process_token_content(t_minishell *sh, const char *start,
							int len, int is_heredoc_delim)
{
	if (is_heredoc_delim)
		return (process_heredoc_token(sh, start, len));
	else
		return (process_regular_token(sh, start, len));
}
