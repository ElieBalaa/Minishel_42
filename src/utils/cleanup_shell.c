/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eal-bala <eal-bala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 21:45:42 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 22:37:27 by eal-bala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_shell(t_minishell *sh)
{
	if (!sh || sh->cleaned_up)
		return ;
	sh->cleaned_up = 1;
	gc_cleanup_all(sh);
	if (sh->is_interactive)
		rl_clear_history();
}
