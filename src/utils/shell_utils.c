/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 21:30:42 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/24 19:44:34 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	heredoc_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		exit(1);
	}
}

int	status_code(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	return (wstatus);
}

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
