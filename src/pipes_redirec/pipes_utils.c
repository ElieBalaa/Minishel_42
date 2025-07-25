/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 00:58:43 by omar-iskand       #+#    #+#             */
/*   Updated: 2025/07/25 21:17:14 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	alloc_pipes(t_pipe_ctx *ctx, int n)
{
	int	i;

	ctx->count = n;
	if (n <= 0)
		return (0);
	ctx->pipes = malloc(sizeof(int [2]) * n);
	if (!ctx->pipes)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (pipe(ctx->pipes[i]))
		{
			while (i--)
			{
				close(ctx->pipes[i][0]);
				close(ctx->pipes[i][1]);
			}
			free(ctx->pipes);
			return (-1);
		}
		++i;
	}
	return (0);
}

void	close_all_pipes(t_pipe_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->count)
	{
		close(ctx->pipes[i][0]);
		close(ctx->pipes[i][1]);
		++i;
	}
}

void	free_pipes(t_pipe_ctx *ctx)
{
	if (ctx->pipes)
	{
		close_all_pipes(ctx);
		free(ctx->pipes);
	}
	ctx->pipes = NULL;
	ctx->count = 0;
}

void	dup_pipes_child(t_pipe_ctx *ctx, int idx, int n)
{
	if (idx > 0)
		dup2(ctx->pipes[idx - 1][0], STDIN_FILENO);
	if (idx < n - 1)
		dup2(ctx->pipes[idx][1], STDOUT_FILENO);
	close_all_pipes(ctx);
}

int	wait_pipeline(pid_t *pids, int n)
{
	int	i;
	int	st;

	st = 0;
	i = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &st, 0) != -1)
		{
		}
		++i;
	}
	if (WIFSIGNALED(st))
		return (128 + WTERMSIG(st));
	if (WIFEXITED(st))
		return (WEXITSTATUS(st));
	return (st);
}
