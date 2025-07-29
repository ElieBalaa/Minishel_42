/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eal-bala <eal-bala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 01:02:13 by omar-iskand       #+#    #+#             */
/*   Updated: 2025/07/28 22:30:18 by eal-bala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_pipeline(t_ast *pipeline, t_ast ***cmds, t_minishell *sh)
{
	int	n;

	n = flatten_pipeline(pipeline, cmds, sh);
	if (n <= 0 || preprocess_heredocs(*cmds, n, sh))
	{
		if (*cmds)
			free(*cmds);
		return (0);
	}
	return (n);
}

int	setup_pipeline_context(t_pipe_ctx *c, int n, t_ast **cmds)
{
	if (alloc_pipes(c, n - 1) < 0)
	{
		free(cmds);
		return (0);
	}
	c->pids = malloc(sizeof(pid_t) * n);
	if (!c->pids)
	{
		free(cmds);
		free_pipes(c);
		return (0);
	}
	return (1);
}

int	handle_pipeline_error(t_ast **cmds, t_pipe_ctx *c)
{
	if (cmds)
		free(cmds);
	if (c->pids)
		free(c->pids);
	free_pipes(c);
	return (1);
}
