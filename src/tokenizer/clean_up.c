/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:57:57 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 20:18:37 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]->text);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	ft_free_split(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
}

void	free_toklist(t_token *lst)
{
	t_token	*next;

	while (lst)
	{
		next = lst->next;
		free(lst);
		lst = next;
	}
}

int	is_quoted(char *str)
{
	int	j;
	int	count_double;
	int	count_single;

	j = 0;
	count_double = 0;
	count_single = 0;
	while (str[j])
	{
		if (str[j] == '"')
			count_double++;
		if (str[j] == '\'')
			count_single++;
		j++;
	}
	if ((count_double && (count_double % 2 == 0))
		|| (count_single && (count_single % 2 == 0)))
		return (1);
	return (0);
}
