/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:16:34 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_loop_content(char *line, int is_last,
		char **temp_content, size_t *temp_size)
{
	if (is_last && !append_temp_content(temp_content, line, temp_size,
			&temp_size[1]))
	{
		free(line);
		return (-1);
	}
	free(line);
	return (1);
}

static int	handle_loop_prompt(int is_piped)
{
	if (!is_piped)
		heredoc_prompt();
	return (1);
}

static int	handle_loop_logic(char **content, char *delimiter,
		int is_last, int is_piped)
{
	char	*line;
	char	*temp_content;
	size_t	params[2];

	line = read_heredoc_line(is_piped);
	if (!line)
		return (0);
	if (check_delimiter_match(line, delimiter))
	{
		free(line);
		if (is_last && temp_content)
			finalize_content(content, &temp_content, &params[0]);
		else
			free(temp_content);
		return (0);
	}
	if (handle_loop_content(line, is_last, &temp_content, params) == -1)
		return (-1);
	return (handle_loop_prompt(is_piped));
}

int	handle_interactive_loop(char **content, char *delimiter,
		int is_last, int is_piped)
{
	while (1)
	{
		if (!handle_loop_logic(content, delimiter, is_last, is_piped))
			break ;
	}
	return (0);
}

int	process_interactive_content(char **content, char *delimiter,
		int is_last, int is_piped)
{
	*content = NULL;
	if (!is_piped)
		heredoc_prompt();
	return (handle_interactive_loop(content, delimiter, is_last, is_piped));
}
