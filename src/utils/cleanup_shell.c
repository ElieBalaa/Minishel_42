/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 21:45:42 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/08 21:45:42 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_shell(t_minishell *sh)
{
	int	i;

	ft_putendl_fd("exit", 1);
	clear_history();
	gc_cleanup_all(sh);
	gc_free_nodes(sh);
	i = 0;
	while (sh->env[i])
		free(sh->env[i++]);
	free(sh->env);
}
