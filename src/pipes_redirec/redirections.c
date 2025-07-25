/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 01:02:07 by omar-iskand       #+#    #+#             */
/*   Updated: 2025/07/25 21:17:26 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_redir(t_ast *node, char op, char *file, t_minishell *sh)
{
	t_redir	*r;
	t_redir	*p;

	r = gc_malloc(sh, sizeof(*r));
	r->op = op;
	r->file = file;
	r->next = NULL;
	if (!node->redirs)
		node->redirs = r;
	else
	{
		p = node->redirs;
		while (p->next)
			p = p->next;
		p->next = r;
	}
}

static void	handle_child_heredoc(t_minishell *sh, char **delims, int *pipefd)
{
	close(pipefd[0]);
	if (process_multiple_heredocs(sh, delims, pipefd) == -1)
		exit(1);
	close(pipefd[1]);
	exit(0);
}

static int	wait_heredoc_child(pid_t pid, int *pipefd)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		close(pipefd[0]);
		return (1);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		return (1);
	}
	return (0);
}

static int	fork_heredoc_process(t_minishell *sh, char **delims, int *pipefd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
		handle_child_heredoc(sh, delims, pipefd);
	return (wait_heredoc_child(pid, pipefd));
}

int	open_all_heredocs(t_ast *node, t_minishell *sh)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 1);
	if (fork_heredoc_process(sh, node->heredoc_delims, pipefd) == 1)
	{
		close(pipefd[0]);
		return (1);
	}
	node->heredoc_fd = pipefd[0];
	return (0);
}
