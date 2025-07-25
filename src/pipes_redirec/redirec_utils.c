/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 00:58:45 by omar-iskand       #+#    #+#             */
/*   Updated: 2025/07/25 21:17:23 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_heredoc_redirection(t_ast *n)
{
	if (n->heredoc_fd >= 0)
	{
		if (dup2(n->heredoc_fd, STDIN_FILENO) == -1)
			return (perror("minishell: dup2"), 1);
		close(n->heredoc_fd);
		n->heredoc_fd = -1;
	}
	return (0);
}

static int	open_redirection_file(t_redir *r)
{
	if (r->op == '<')
		return (open(r->file, O_RDONLY));
	else if (r->op == '>')
		return (open(r->file, O_CREAT | O_WRONLY | O_TRUNC, 0644));
	else
		return (open(r->file, O_CREAT | O_WRONLY | O_APPEND, 0644));
}

static int	handle_redirection_error(t_redir *r)
{
	write(2, "minishell: ", 11);
	write(2, r->file, ft_strlen(r->file));
	if (r->op == '<')
		write(2, ": No such file or directory\n", 28);
	else
		write(2, ": Permission denied\n", 20);
	return (1);
}

static int	process_single_redirection(t_redir *r)
{
	int	fd;
	int	target_fd;

	fd = open_redirection_file(r);
	if (r->op == '<')
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (fd < 0 || dup2(fd, target_fd) < 0)
		return (handle_redirection_error(r));
	close(fd);
	return (0);
}

int	apply_redirections(t_ast *n)
{
	t_redir	*r;

	r = n->redirs;
	if (handle_heredoc_redirection(n))
		return (1);
	while (r)
	{
		if (process_single_redirection(r))
			return (1);
		r = r->next;
	}
	return (0);
}
