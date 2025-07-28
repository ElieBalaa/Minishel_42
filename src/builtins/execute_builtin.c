/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:25 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 14:08:16 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_set(t_minishell *sh, const char *str)
{
	char	*equal;
	size_t	key_len;
	int		i;

	equal = ft_strchr(str, '=');
	if (!sh || !str || !equal)
		return (1);
	key_len = (size_t)(equal - str);
	i = -1;
	while (sh->env[++i])
	{
		if (!ft_strncmp(sh->env[i], str, key_len)
			&& (sh->env[i][key_len] == '='
			|| sh->env[i][key_len] == '\0'))
		{
			sh->env[i] = gc_strdup(sh, str);
			if (sh->env[i])
				return (0);
			else
				return (1);
		}
	}
	return (extend_env_array(sh, str, i));
}

int	builtin_dot(t_minishell *sh, char **args)
{
	(void)sh;
	if (!args[1])
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		return (2);
	}
	return (0);
}

int	execute_builtin(t_minishell *sh, char **args)
{
	if (!args || !args[0])
		return (-1);
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(sh, args));
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(sh, args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(sh, args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(sh, args));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(sh, args));
	if (!ft_strcmp(args[0], "export"))
		return (builtin_export(sh, args));
	if (!ft_strcmp(args[0], "unset"))
		return (builtin_unset(sh, args));
	if (!ft_strcmp(args[0], "."))
		return (builtin_dot(sh, args));
	return (-1);
}
