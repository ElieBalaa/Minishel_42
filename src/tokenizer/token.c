/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:44:21 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:18:42 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	fill_segment_args(t_fill_args *args)
{
	int	i;
	int	arg_idx;

	i = 0;
	arg_idx = 0;
	while (i < args->n && ft_strcmp(args->tokens[i], "|"))
	{
		if (is_redir(args->tokens[i]))
		{
			if (process_redir(args->tokens, &i, args->node, args->sh) == -1)
				return (-1);
		}
		else
		{
			args->argv[arg_idx] = gc_strdup(args->sh, args->tokens[i]);
			arg_idx++;
		}
		i++;
	}
	args->argv[arg_idx] = NULL;
	return (0);
}

t_ast	*init_ast_node(t_minishell *sh)
{
	t_ast	*ast;

	ast = gc_malloc(sh, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->cmd = NULL;
	ast->left = NULL;
	ast->right = NULL;
	ast->input = NULL;
	ast->output = NULL;
	ast->heredoc_delims = NULL;
	ast->append = 0;
	ast->is_heredoc = 0;
	ast->redirs = NULL;
	return (ast);
}

static t_ast	*handle_syntax_error(char **tokens, int n, t_minishell *sh)
{
	ft_putstr_fd("minishell:"
		" syntax error near unexpected token `", STDERR_FILENO);
	if (n > 0 && tokens[n - 1])
		ft_putstr_fd(tokens[n - 1], STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	if (sh)
		sh->last_exit = 2;
	return (NULL);
}

t_ast	*parse_segment(char **tokens, int n, t_minishell *sh)
{
	t_ast		*node;
	t_fill_args	args;
	int			argc;

	node = init_ast_node(sh);
	if (!node)
		return (NULL);
	argc = count_args(tokens, n);
	if (argc == -1)
		return (handle_syntax_error(tokens, n, sh));
	if (argc == 0)
		return (NULL);
	args.argv = gc_malloc(sh, sizeof(*args.argv) * (argc + 1));
	if (!args.argv)
		return (NULL);
	args = (t_fill_args){tokens, n, args.argv, node, sh};
	if (fill_segment_args(&args) == -1)
		return (NULL);
	node->cmd = args.argv;
	return (node);
}
