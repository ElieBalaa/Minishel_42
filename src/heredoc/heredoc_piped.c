/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_piped.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/27 18:48:59 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_heredoc_line(t_delimiter_content_params *params,
		char *line, int is_quoted)
{
	if (!is_quoted)
	{
		if (!append_expanded_line(params->sh, line, params->vars))
			return (-1);
	}
	else
	{
		if (!append_raw_line(params->sh, line, params->vars))
			return (-1);
	}
	return (0);
}

static int	process_heredoc_line(t_delimiter_content_params *params,
		char *line, int is_quoted, void (*old_handler)(int))
{
	if (params->is_last)
	{
		if (handle_heredoc_line(params, line, is_quoted) == -1)
		{
			free(line);
			signal(SIGINT, old_handler);
			return (-1);
		}
	}
	free(line);
	return (0);
}

static int	heredoc_piped_loop(t_delimiter_content_params *params,
		char *clean_delim, int is_quoted, void (*old_handler)(int))
{
	char	*line;

	while (1)
	{
		if (!params->is_piped)
			heredoc_prompt();
		line = read_heredoc_line(params->is_piped);
		if (!line)
		{
			signal(SIGINT, old_handler);
			return (-1);
		}
		if (check_delimiter_match(line, clean_delim))
		{
			free(line);
			break ;
		}
		if (process_heredoc_line(params, line, is_quoted, old_handler) == -1)
			return (-1);
	}
	return (0);
}

static int	process_heredoc_content(t_delimiter_content_params *params,
		char *clean_delim, int is_quoted)
{
	void	(*old_handler)(int);
	int		ret;

	old_handler = signal(SIGINT, heredoc_sig_handler);
	ret = heredoc_piped_loop(params, clean_delim, is_quoted, old_handler);
	signal(SIGINT, old_handler);
	return (ret);
}

int	process_delimiter_content(t_delimiter_content_params *params)
{
	char	*clean_delim;
	int		is_quoted;

	is_quoted = is_originally_quoted(params->delimiter);
	if (is_quoted)
		clean_delim = ft_strdup(params->delimiter + 1);
	else
		clean_delim = ft_strdup(params->delimiter);
	if (process_heredoc_content(params, clean_delim, is_quoted) == -1)
	{
		free(clean_delim);
		return (-1);
	}
	free(clean_delim);
	return (0);
}
