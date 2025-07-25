/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:12:42 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:19:02 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	setup_signals(void)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	process_line(t_minishell *sh, char *line)
{
	t_ast	*ast;

	if (!line || !*line)
		return ;
	if (process_logical_operators(sh, line))
		return ;
	if (!quotes_balanced(line))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", STDERR_FILENO);
		sh->last_exit = 2;
		return ;
	}
	ast = parse_line(line, sh);
	if (!ast)
		return ;
	sh->last_exit = execute_ast(sh, ast);
}

static int	run_command_mode(t_minishell *sh, int ac, char **av)
{
	if (ac == 3 && !ft_strcmp(av[1], "-c"))
	{
		process_line(sh, av[2]);
		return (1);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	sh;

	init_minishell(&sh, envp);
	setup_signals();
	if (!run_command_mode(&sh, ac, av))
		repl(&sh);
	gc_cleanup_all(&sh);
	return (sh.last_exit);
}
