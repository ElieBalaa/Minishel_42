/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_gc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 00:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/27 22:47:07 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_minishell	*g_shell_ctx = NULL;

static void	cleanup_readline(void)
{
	rl_clear_history();
}

static void	gc_emergency_cleanup(int sig)
{
	cleanup_readline();
	if (g_shell_ctx)
		gc_cleanup_all(g_shell_ctx);
	exit(sig);
}

int	gc_init(t_minishell *sh)
{
	if (!sh)
		return (0);
	sh->gc.head = NULL;
	sh->gc.count = 0;
	g_shell_ctx = sh;
	signal(SIGINT, gc_emergency_cleanup);
	signal(SIGTERM, gc_emergency_cleanup);
	return (1);
}

int	gc_add(t_minishell *sh, void *ptr)
{
	t_gc_node	*new_node;

	if (!ptr || !sh)
		return (0);
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
		return (0);
	new_node->ptr = ptr;
	new_node->next = sh->gc.head;
	sh->gc.head = new_node;
	sh->gc.count++;
	return (1);
}
