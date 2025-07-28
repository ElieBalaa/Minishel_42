/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:17:49 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:17:55 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*process_quoted_delimiter(t_minishell *sh, const char *start, int len)
{
	char	*clean;

	clean = gc_strndup(sh, start + 1, len - 2);
	return (gc_strjoin(sh, "'", clean));
}
