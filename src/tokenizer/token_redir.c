/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:11:09 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:18:30 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_output_redir(char **tok,
		int *i, t_ast *node, t_minishell *sh)
{
	int		is_append;
	char	redir_char;

	is_append = (ft_strcmp(tok[*i], ">>") == 0);
	(*i)++;
	node->output = gc_strdup(sh, tok[*i]);
	if (is_append)
		node->append = 1;
	if (is_append)
		redir_char = '+';
	else
		redir_char = '>';
	add_redir(node, redir_char, gc_strdup(sh, tok[*i]), sh);
}

static int	is_redirection_operator(char *token)
{
	return (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0);
}

static int	handle_redir_error(char **tok,
	int *i, t_minishell *sh, int is_heredoc)
{
	ft_putstr_fd("minishell:"
		" syntax error near unexpected token `", STDERR_FILENO);
	if (tok[*i + 1])
		ft_putstr_fd(tok[*i + 1], STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	if (sh)
		sh->last_exit = 2;
	if (is_heredoc)
		return (-1);
	return (127);
}

static int	process_redirection(char **tok,
	int *i, t_ast *node, t_minishell *sh)
{
	if (ft_strcmp(tok[*i], "<") == 0)
	{
		if (!tok[*i + 1] || is_redirection_operator(tok[*i + 1]))
			return (handle_redir_error(tok, i, sh, 0));
		process_input_redir(tok, i, node, sh);
	}
	else if (ft_strcmp(tok[*i], "<<") == 0)
	{
		if (!tok[*i + 1] || is_redirection_operator(tok[*i + 1]))
			return (handle_redir_error(tok, i, sh, 1));
		process_heredoc_redir(sh, tok, i, node);
	}
	else if (ft_strcmp(tok[*i], ">") == 0 || ft_strcmp(tok[*i], ">>") == 0)
	{
		if (!tok[*i + 1] || is_redirection_operator(tok[*i + 1]))
			return (handle_redir_error(tok, i, sh, 1));
		process_output_redir(tok, i, node, sh);
	}
	return (0);
}

int	process_redir(char **tok,
	int *i, t_ast *node, t_minishell *sh)
{
	if (!tok || !tok[*i] || !node || *i < 0)
		return (0);
	return (process_redirection(tok, i, node, sh));
}
