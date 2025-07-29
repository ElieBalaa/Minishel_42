/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:16:19 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:14:49 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**create_new_env(t_minishell *sh, const char *key, int old)
{
	int		i;
	int		j;
	char	**new;

	(void)sh;
	new = gc_malloc(sh, sizeof(*new) * (old + 1));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (sh->env[i])
	{
		if (!(ft_strncmp(sh->env[i], key, ft_strlen(key)) == 0
				&& sh->env[i][ft_strlen(key)] == '='))
		{
			new[j] = gc_strdup(sh, sh->env[i]);
			j++;
		}
		i++;
	}
	new[j] = NULL;
	return (new);
}

void	env_unset(t_minishell *sh, const char *key)
{
	int		old;
	char	**new;

	old = env_count(sh->env);
	new = create_new_env(sh, key, old);
	if (!new)
		return ;
	sh->env = new;
}

int	builtin_unset(t_minishell *sh, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (is_valid_key(args[i]))
			env_unset(sh, args[i]);
		i++;
	}
	return (0);
}
