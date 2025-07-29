/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:14:56 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

static void	expand_exit_status_markers(t_minishell *sh, char **args)
{
	int		i;
	char	*temp_val;
	char	*val;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		if (strstr(args[i], "$$EXIT_STATUS$$"))
		{
			temp_val = ft_itoa(sh->last_exit);
			val = gc_strdup(sh, temp_val);
			free(temp_val);
			args[i] = val;
		}
		i++;
	}
}

int	fork_and_execute(t_minishell *sh, const char *p, char **a, t_ast *n)
{
	pid_t	pid;
	int		st;

	(void)p;
	(void)a;
	expand_exit_status_markers(sh, n->cmd);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (!pid)
	{
		if (apply_redirections(n))
			exit(1);
		exec_node(sh, n);
		exit(sh->last_exit);
	}
	ignore_signals();
	waitpid(pid, &st, 0);
	setup_signals();
	if (WIFSIGNALED(st))
		sh->last_exit = 128 + WTERMSIG(st);
	else if (WIFEXITED(st))
		sh->last_exit = WEXITSTATUS(st);
	return (sh->last_exit);
}

static int	parent_builtin(char *s)
{
	return (!ft_strcmp(s, "cd") || !ft_strcmp(s, "export")
		|| !ft_strcmp(s, "unset") || !ft_strcmp(s, "exit"));
}

static int	run_parent_builtin(t_minishell *sh, t_ast *n)
{
	int		in;
	int		out;
	char	*underscore_entry;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	if (apply_redirections(n))
		return (sh->last_exit = 1, dup2(in, 0),
			dup2(out, 1), close(in), close(out), 1);
	expand_exit_status_markers(sh, n->cmd);
	underscore_entry = gc_strjoin(sh, "_=", n->cmd[0]);
	env_set(sh, underscore_entry);
	sh->last_exit = execute_builtin(sh, n->cmd);
	dup2(in, 0);
	dup2(out, 1);
	close(in);
	close(out);
	return (sh->last_exit);
}

int	execute_ast(t_minishell *sh, t_ast *ast)
{
	if (!ast)
		return (1);
	if (ast->left || ast->right)
		return (execute_pipeline(sh, ast));
	ast->heredoc_fd = -1;
	if (ast->is_heredoc && ast->heredoc_delims)
		if (open_all_heredocs(ast, sh))
			return (1);
	if (!ast->cmd || !ast->cmd[0])
		return (0);
	if (parent_builtin(ast->cmd[0]))
		return (run_parent_builtin(sh, ast));
	return (fork_and_execute(sh, NULL, ast->cmd, ast));
}
