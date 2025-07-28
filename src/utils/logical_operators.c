/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 20:56:46 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

static void	execute_logical_commands(t_minishell *sh, char *first_cmd,
	char *second_cmd, int is_or)
{
	process_line(sh, first_cmd);
	if ((is_or && sh->last_exit != 0) || (!is_or && sh->last_exit == 0))
		process_line(sh, second_cmd);
}

int	process_logical_operators(t_minishell *sh, char *line)
{
	char	*split_pos;
	int		is_or;

	split_pos = find_or_operator(line, &is_or);
	if (!split_pos)
		return (0);
	*split_pos = '\0';
	execute_logical_commands(sh, line, split_pos + 4, is_or);
	return (1);
}
