/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:20:46 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:14:41 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	extend_env_array(t_minishell *sh, const char *str, int i)
{
	char	**new_env;
	int		j;

	new_env = gc_malloc(sh, sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	j = 0;
	while (j < i)
	{
		new_env[j] = gc_strdup(sh, sh->env[j]);
		++j;
	}
	new_env[i] = gc_strdup(sh, str);
	new_env[i + 1] = NULL;
	sh->env = new_env;
	return (0);
}

static void	write_escaped_value(char *value)
{
	size_t	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '\\')
			write(1, "\\\\", 2);
		else
			write(1, &value[i], 1);
		++i;
	}
}

static void	print_env_entry(char *e)
{
	size_t	i;

	i = 0;
	while (e[i] && e[i] != '=')
		++i;
	write(1, "declare -x ", 11);
	write(1, e, i);
	if (e[i] == '=')
	{
		write(1, "=\"", 2);
		write_escaped_value(e + i + 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

int	builtin_export(t_minishell *sh, char **av)
{
	int		err;
	int		i;
	char	**sorted;

	err = 0;
	if (!av[1])
	{
		sorted = env_copy(sh->env);
		if (!sorted)
			return (1);
		sort_strings(sorted);
		i = -1;
		while (sorted[++i])
			print_env_entry(sorted[i]);
		free_env_strings(sorted);
		free(sorted);
		return (0);
	}
	i = 1;
	while (av[i])
	{
		err |= process_export_arg(sh, av[i]);
		++i;
	}
	return (err);
}
