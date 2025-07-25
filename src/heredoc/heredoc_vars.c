/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/01/14 15:00:00 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_heredoc_vars(char **delimiters, int *i, int *last_index,
		int *is_piped)
{
	*i = 0;
	*last_index = find_last_delimiter_index(delimiters);
	if (*last_index < 0)
		return (-1);
	*is_piped = !isatty(STDIN_FILENO);
	return (0);
}
