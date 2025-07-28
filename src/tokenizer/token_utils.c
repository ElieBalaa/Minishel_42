/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:11:09 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 15:07:12 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&' || c == ';');
}

int	op_len(const char *s)
{
	if (!s)
		return (0);
	if ((s[0] == '<' && s[1] == '<') || (s[0] == '>' && s[1] == '>')
		|| (s[0] == '|' && s[1] == '|') || (s[0] == '&' && s[1] == '&'))
		return (2);
	if (s[0] == '<' || s[0] == '>' || s[0] == '|' || s[0] == ';')
		return (1);
	return (0);
}

int	is_semicolon_error(char *token)
{
	return (ft_strcmp(token, ";;") == 0 || ft_strcmp(token, ";") == 0);
}

t_token	*tok_last(t_token *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}
