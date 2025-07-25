/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:10:13 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/24 19:51:34 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_prompt(void)
{
	write(STDOUT_FILENO, "heredoc> ", 9);
}

int	find_last_delimiter_index(char **delimiters)
{
	int	i;

	if (!delimiters)
		return (-1);
	i = 0;
	while (delimiters[i])
		i++;
	return (i - 1);
}

static int	heredoc_loop_body(t_heredoc_loop_params *params)
{
	int							i;
	t_delimiter_content_params	delim_params;

	i = 0;
	while (params->delimiters[i])
	{
		delim_params.sh = params->sh;
		delim_params.delimiter = params->delimiters[i];
		delim_params.is_piped = params->is_piped;
		delim_params.is_last = (i == params->last_index);
		delim_params.vars = params->vars;
		if (process_delimiter_content(&delim_params) == -1)
		{
			free(params->vars->content);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	preprocess_heredocs(t_ast **cmds, int n, t_minishell *sh)
{
	int	i;

	i = 0;
	while (i < n)
	{
		cmds[i]->heredoc_fd = -1;
		if (cmds[i]->is_heredoc && cmds[i]->heredoc_delims)
			if (open_all_heredocs(cmds[i], sh))
				return (1);
		++i;
	}
	return (0);
}

int	process_multiple_heredocs(t_minishell *sh, char **delimiters,
		int *pipe_fd)
{
	int						i;
	int						last_index;
	int						is_piped;
	t_content_vars			vars;
	t_heredoc_loop_params	params;

	if (!sh || !delimiters || !pipe_fd)
		return (-1);
	if (init_heredoc_vars(delimiters, &i, &last_index, &is_piped) == -1)
		return (-1);
	init_content_vars(&vars);
	params.sh = sh;
	params.delimiters = delimiters;
	params.is_piped = is_piped;
	params.last_index = last_index;
	params.vars = &vars;
	if (heredoc_loop_body(&params) == -1)
		return (-1);
	if (vars.content)
	{
		write(pipe_fd[1], vars.content, vars.size);
		free(vars.content);
	}
	return (0);
}
