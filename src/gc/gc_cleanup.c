/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:16:04 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*gc_realloc(t_minishell *sh,
	void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (!sh)
		return (NULL);
	if (!ptr)
		return (gc_malloc(sh, new_size));
	if (new_size == 0)
		return (NULL);
	new_ptr = gc_malloc(sh, new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size > 0)
	{
		if (old_size > new_size)
			ft_memcpy(new_ptr, ptr, new_size);
		else
			ft_memcpy(new_ptr, ptr, old_size);
	}
	return (new_ptr);
}

void	gc_cleanup_all(t_minishell *sh)
{
	t_gc_node	*current;
	t_gc_node	*next;

	if (!sh)
		return ;
	current = sh->gc.head;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	sh->gc.head = NULL;
	sh->gc.count = 0;
}

void	gc_free_nodes(t_minishell *sh)
{
	t_gc_node	*current;
	t_gc_node	*next;

	if (!sh)
		return ;
	current = sh->gc.head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	sh->gc.head = NULL;
	sh->gc.count = 0;
}
