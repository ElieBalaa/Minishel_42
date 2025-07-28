/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 01:02:13 by omar-iskand       #+#    #+#             */
/*   Updated: 2025/07/25 21:17:20 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_child_process(t_child_params p)
{
	p.c->pids[p.i] = fork();
	if (p.c->pids[p.i] < 0)
		return (free_pipes(p.c), perror("fork"), 1);
	if (!p.c->pids[p.i])
	{
		dup_pipes_child(p.c, p.i, p.n);
		if (apply_redirections(p.cmd))
			exit(EXIT_FAILURE);
		exec_node(p.sh, p.cmd);
		exit(p.sh->last_exit);
	}
	return (0);
}

static int	execute_commands(t_minishell *sh,
	t_ast **cmds, int n, t_pipe_ctx *c)
{
	int				i;
	t_child_params	p;

	i = -1;
	p = (t_child_params){sh, NULL, c, 0, n};
	while (++i < n)
	{
		p.cmd = cmds[i];
		p.i = i;
		if (handle_child_process(p))
			return (1);
	}
	return (0);
}

static int	cleanup_and_wait(t_minishell *sh,
	t_pipe_ctx *c, int n, t_ast **cmds)
{
	close_all_pipes(c);
	ignore_signals();
	sh->last_exit = wait_pipeline(c->pids, n);
	setup_signals();
	free(c->pids);
	free_pipes(c);
	free(cmds);
	return (sh->last_exit);
}

int	execute_pipeline(t_minishell *sh, t_ast *pipeline)
{
	t_ast		**cmds;
	int			n;
	t_pipe_ctx	c;

	n = validate_pipeline(pipeline, &cmds);
	if (!n)
		return (1);
	if (!setup_pipeline_context(&c, n, cmds))
		return (perror("pipe"), 1);
	if (execute_commands(sh, cmds, n, &c))
		return (handle_pipeline_error(cmds, &c));
	return (cleanup_and_wait(sh, &c, n, cmds));
}
