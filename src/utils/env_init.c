/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 18:00:00 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	copy_env_vars(t_minishell *sh, char **envp, size_t n)
{
	size_t	i;

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

void	set_initial_env_vars(t_minishell *sh)
{
	char	*cwd;
	char	*pwd_entry;
	char	*underscore_entry;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		pwd_entry = gc_strjoin(sh, "PWD=", cwd);
		env_set(sh, pwd_entry);
		free(cwd);
	}
	underscore_entry = gc_strjoin(sh, "_=", "/usr/bin/env");
	env_set(sh, underscore_entry);
}
