/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 21:19:03 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/23 21:29:12 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_till_pipe(char **words)
{
	int	count;

	count = 0;
	while (words[count] && ft_strcmp(words[count], "|") != 0)
		count++;
	return (count);
}

static t_ast	*handle_pipe_error(t_minishell *sh)
{
	ft_putstr_fd("minishell:"
		" syntax error near unexpected token `|'\n", STDERR_FILENO);
	if (sh)
		sh->last_exit = 2;
	return (NULL);
}

static t_ast	*process_pipeline_segment(t_pipeline_params params)
{
	t_ast	*pipe_node;

	if (!params.words[*params.i + 1]
		|| ft_strcmp(params.words[*params.i + 1], "|") == 0)
		return (handle_pipe_error(params.sh));
	if (!*params.node)
		*params.node = parse_segment(params.words, *params.i, params.sh);
	else
		(*params.node)->right = parse_segment(params.words + *params.start,
				*params.i - *params.start, params.sh);
	if (!*params.node)
		return (NULL);
	*params.start = *params.i + 1;
	pipe_node = init_ast_node(params.sh);
	pipe_node->left = *params.node;
	pipe_node->right = parse_segment(params.words + *params.start,
			count_till_pipe(params.words + *params.start), params.sh);
	if (!pipe_node->right)
		return (NULL);
	*params.node = pipe_node;
	return (*params.node);
}

t_ast	*parse_pipeline(char **words, t_minishell *sh)
{
	t_ast				*node;
	int					i;
	int					start;
	t_pipeline_params	params;

	i = 0;
	start = 0;
	node = NULL;
	if (words[0] && ft_strcmp(words[0], "|") == 0)
		return (handle_pipe_error(sh));
	params = (t_pipeline_params){words, &i, &start, &node, sh};
	while (words[i])
	{
		if (ft_strcmp(words[i], "|") == 0)
		{
			if (!process_pipeline_segment(params))
				return (NULL);
		}
		i++;
	}
	if (!node)
		node = parse_segment(words, i, sh);
	return (node);
}
