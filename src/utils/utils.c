/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/22 23:32:32 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	free(ptr);
	return (new_ptr);
}

static void	set_shlvl(t_minishell *sh, int lvl)
{
	char	*num;
	char	*entry;

	num = ft_itoa(lvl);
	if (!num)
		return ;
	entry = gc_strjoin(sh, "SHLVL=", num);
	free(num);
	if (entry)
		env_set(sh, entry);
}

void	shlvl(t_minishell *sh)
{
	char	*cur;
	int		lvl;

	cur = get_env_var(sh, "SHLVL");
	if (!cur)
	{
		env_set(sh, "SHLVL=1");
		return ;
	}
	lvl = ft_atoi(cur) + 1;
	if (lvl < 0)
		lvl = 0;
	else if (lvl > 1000)
	{
		ft_putstr_fd(
			"minishell: shlvl too high, resetting to 1\n", STDERR_FILENO);
		lvl = 1;
	}
	set_shlvl(sh, lvl);
}
