/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_heredoc_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:18:11 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:18:14 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	copy_existing_delims(char **new_delims,
	char **heredoc_delims, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		new_delims[j] = heredoc_delims[j];
		j++;
	}
}

char	*prepare_quoted_delimiter(char *delimiter)
{
	char	*clean_delim;
	char	*quoted_marker;

	clean_delim = remove_quotes(delimiter);
	quoted_marker = ft_strjoin("'", clean_delim);
	free(clean_delim);
	return (quoted_marker);
}

void	add_heredoc_delimiter(t_ast *node, char *delimiter)
{
	int		count;
	char	**new_delims;
	char	*clean_delim;
	int		is_quoted;

	count = 0;
	while (node->heredoc_delims[count])
		count++;
	new_delims = malloc((count + 2) * sizeof(char *));
	if (!new_delims)
		return ;
	copy_existing_delims(new_delims, node->heredoc_delims, count);
	is_quoted = is_quoted_delimiter(delimiter);
	if (is_quoted)
		clean_delim = prepare_quoted_delimiter(delimiter);
	else
		clean_delim = ft_strdup(delimiter);
	new_delims[count] = clean_delim;
	new_delims[count + 1] = NULL;
	free(node->heredoc_delims);
	node->heredoc_delims = new_delims;
}
