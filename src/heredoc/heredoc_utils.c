/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/27 18:49:10 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_line(char **current_pos)
{
	char	*start;
	char	*end;
	char	*line;
	int		len;

	if (!current_pos || !*current_pos)
		return (NULL);
	start = *current_pos;
	end = ft_strchr(start, '\n');
	if (end)
	{
		len = end - start;
		line = ft_strndup(start, len);
		*current_pos = end + 1;
	}
	else
	{
		len = ft_strlen(start);
		line = ft_strndup(start, len);
		*current_pos = NULL;
	}
	return (line);
}

int	check_delimiter_match(char *line, char *delimiter)
{
	size_t	len;

	if (!line || !delimiter)
		return (0);
	len = ft_strlen(delimiter);
	return (!ft_strncmp(line, delimiter, len)
		&& (line[len] == '\0' || line[len] == '\n'));
}

int	append_content_line(char **content, char *line, size_t *content_size,
		size_t *content_capacity)
{
	size_t	line_len;
	size_t	new_capacity;
	char	*new_content;

	if (!content || !line || !content_size || !content_capacity)
		return (0);
	line_len = ft_strlen(line);
	if (*content_size + line_len + 1 >= *content_capacity)
	{
		if (*content_capacity == 0)
			new_capacity = 1024;
		else
			new_capacity = *content_capacity * 2;
		new_content = malloc(new_capacity);
		if (*content && *content_size > 0)
			ft_memcpy(new_content, *content, *content_size);
		if (*content)
			free(*content);
		*content = new_content;
		*content_capacity = new_capacity;
	}
	ft_memcpy(*content + *content_size, line, line_len);
	*content_size += line_len;
	(*content)[*content_size] = '\0';
	return (1);
}

int	append_temp_content(char **temp_content, char *line, size_t *temp_size,
		size_t *temp_capacity)
{
	return (append_content_line(temp_content, line, temp_size, temp_capacity));
}

void	finalize_content(char **content, char **temp_content, size_t *temp_size)
{
	if (!content || !temp_content)
		return ;
	*content = *temp_content;
	*temp_content = NULL;
	(void)temp_size;
}
