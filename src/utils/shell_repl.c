/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_repl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:10:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 20:42:18 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../../includes/minishell.h"

// static void	handle_readline_input(t_minishell *sh, char *line,
// 		char **last_line)
// {
// 	if (*line)
// 	{
// 		process_line(sh, line);
// 		gc_cleanup_all(sh);
// 	}
// 	if (sh->is_interactive && !*last_line)
// 		add_history(line);
// 	else if (sh->is_interactive && *last_line && ft_strcmp(*last_line, line))
// 	{
// 		add_history(line);
// 		free(*last_line);
// 		*last_line = ft_strdup(line);
// 	}
// 	else if (sh->is_interactive)
// 	{
// 		if (*last_line)
// 			free(*last_line);
// 		*last_line = ft_strdup(line);
// 	}
// }

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
