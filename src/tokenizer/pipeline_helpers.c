/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 21:19:03 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/23 21:29:12 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast	*create_pipe_node(t_pipeline_params params)
{
	t_ast	*pipe_node;

	*params.start = *params.i + 1;
	pipe_node = init_ast_node(params.sh);
	pipe_node->left = *params.node;
	pipe_node->right = parse_segment(params.words + *params.start,
			count_till_pipe(params.words + *params.start), params.sh);
	if (!pipe_node->right)
	{
		free(pipe_node);
		return (NULL);
	}
	*params.node = pipe_node;
	return (*params.node);
}

static t_ast	*handle_existing_node(t_pipeline_params params)
{
	if (!*params.node)
		*params.node = parse_segment(params.words, *params.i, params.sh);
	else
	{
		if ((*params.node)->right)
			free_ast((*params.node)->right);
		(*params.node)->right = parse_segment(params.words + *params.start,
				*params.i - *params.start, params.sh);
	}
	return (*params.node);
}

t_ast	*process_pipeline_segment(t_pipeline_params params)
{
	if (!params.words[*params.i + 1]
		|| ft_strcmp(params.words[*params.i + 1], "|") == 0)
		return (handle_pipe_error(params.sh));
	if (!handle_existing_node(params))
		return (NULL);
	return (create_pipe_node(params));
}
