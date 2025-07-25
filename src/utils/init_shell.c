/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 21:40:42 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/23 21:30:40 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	repl(t_minishell *sh)
{
	char		*line;
	static char	*last_line;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (*line)
			process_line(sh, line);
		if (!last_line)
			add_history(line);
		if (*line && last_line)
		{
			if (ft_strcmp(line, last_line) != 0)
				add_history(line);
		}
		last_line = gc_strdup(sh, line);
		free(line);
	}
}

static void	init_environment(t_minishell *sh, char **envp)
{
	size_t	n;
	size_t	i;

	n = 0;
	while (envp[n])
		n++;
	sh->env = gc_malloc(sh, sizeof(char *) * (n + 10));
	if (!sh->env)
	{
		gc_cleanup_all(sh);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < n)
	{
		sh->env[i] = gc_strdup(sh, envp[i]);
		if (!sh->env[i])
		{
			gc_cleanup_all(sh);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	sh->env[i] = NULL;
}

void	init_minishell(t_minishell *sh, char **envp)
{
	if (!sh || !envp)
		exit(EXIT_FAILURE);
	if (!gc_init(sh))
		exit(EXIT_FAILURE);
	init_environment(sh, envp);
	sh->last_exit = 0;
	sh->is_interactive = isatty(STDIN_FILENO);
	sh->child_pid = -1;
	sh->heredoc_active = 0;
	shlvl(sh);
}
