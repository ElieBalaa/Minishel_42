/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 20:56:46 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_quotes_at_position(char *line, char *pos)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (line + i < pos)
	{
		if (line[i] == '\'' && !in_double)
			in_single ^= 1;
		else if (line[i] == '"' && !in_single)
			in_double ^= 1;
		else if (line[i] == '\\' && line[i + 1] && !in_single)
			++i;
		++i;
	}
	return (in_single || in_double);
}

char	*find_or_operator(char *line, int *is_or)
{
	char	*or_pos;
	char	*and_pos;

	or_pos = ft_strnstr(line, " || ", ft_strlen(line));
	and_pos = ft_strnstr(line, " && ", ft_strlen(line));
	if (!or_pos && !and_pos)
		return (NULL);
	if (or_pos && (!and_pos || or_pos < and_pos))
	{
		if (check_quotes_at_position(line, or_pos))
			return (NULL);
		*is_or = 1;
		return (or_pos);
	}
	else if (and_pos)
	{
		if (check_quotes_at_position(line, and_pos))
			return (NULL);
		*is_or = 0;
		return (and_pos);
	}
	return (NULL);
}
