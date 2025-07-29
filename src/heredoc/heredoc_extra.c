/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/27 18:47:42 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_originally_quoted(const char *delimiter)
{
	if (delimiter && delimiter[0] == '\'')
		return (1);
	return (0);
}

char	*read_heredoc_line(int is_piped)
{
	char	*line;
	size_t	len;
	ssize_t	read_result;

	if (is_piped)
	{
		line = NULL;
		len = 0;
		read_result = getline(&line, &len, stdin);
		if (read_result == -1)
		{
			free(line);
			return (NULL);
		}
		if (line && line[read_result - 1] == '\n')
			line[read_result - 1] = '\0';
		return (line);
	}
	else
	{
		line = readline("> ");
		if (!line)
			return (NULL);
		return (line);
	}
}

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
