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

char	*get_colored_prompt(t_minishell *sh)
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

static void	init_environment(t_minishell *sh, char **envp)
{
	size_t	n;

	n = 0;
	while (envp[n])
		n++;
	sh->env = gc_malloc(sh, sizeof(char *) * (n + 10));
	if (!sh->env)
	{
		gc_cleanup_all(sh);
		exit(EXIT_FAILURE);
	}
	copy_env_vars(sh, envp, n);
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
	sh->cleaned_up = 0;
	set_initial_env_vars(sh);
	shlvl(sh);
}
