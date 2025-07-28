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

static char	*create_display_path(t_minishell *sh, char *cwd)
{
	char	*home;
	char	*display_path;

	home = getenv("HOME");
	if (home && ft_strncmp(cwd, home, ft_strlen(home)) == 0)
	{
		display_path = gc_malloc(sh, ft_strlen(cwd) - ft_strlen(home) + 2);
		display_path[0] = '~';
		ft_strcpy(display_path + 1, cwd + ft_strlen(home));
	}
	else
		display_path = gc_strdup(sh, cwd);
	return (display_path);
}

static char	*get_colored_prompt(t_minishell *sh)
{
	char	*cwd;
	char	*display_path;
	char	*temp;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (gc_strdup(sh, "🐚 mini-shell ➤ "));
	display_path = create_display_path(sh, cwd);
	temp = gc_strjoin(sh, "\033[1;36m🐚 \033[1;35mmini-shell\033[0m \033[1;33m",
			display_path);
	prompt = gc_strjoin(sh, temp, "\033[1;32m ➤ \033[0m");
	free(cwd);
	return (prompt);
}

void	repl(t_minishell *sh)
{
	char		*line;
	char		*prompt;
	static char	*last_line;

	while (1)
	{
		prompt = get_colored_prompt(sh);
		line = readline(prompt);
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
