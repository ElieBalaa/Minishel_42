/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 00:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:16:08 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*gc_malloc(t_minishell *sh, size_t size)
{
	void	*ptr;

	if (!sh)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (!gc_add(sh, ptr))
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}

char	*gc_strdup(t_minishell *sh, const char *s)
{
	char	*dup;

	if (!s || !sh)
		return (NULL);
	dup = ft_strdup(s);
	if (!dup)
		return (NULL);
	if (!gc_add(sh, dup))
	{
		free(dup);
		return (NULL);
	}
	return (dup);
}

char	*ftt_strndup(const char *s, int n)
{
	int		i;
	int		len;
	char	*dup;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*gc_strndup(t_minishell *sh, const char *s, int n)
{
	char	*dup;

	if (!s || !sh)
		return (NULL);
	dup = ftt_strndup(s, n);
	if (!dup)
		return (NULL);
	if (!gc_add(sh, dup))
	{
		free(dup);
		return (NULL);
	}
	return (dup);
}

char	*gc_strjoin(t_minishell *sh, const char *s1, const char *s2)
{
	char	*joined;

	if (!s1 || !s2 || !sh)
		return (NULL);
	joined = ft_strjoin((char *)s1, (char *)s2);
	if (!joined)
		return (NULL);
	if (!gc_add(sh, joined))
	{
		free(joined);
		return (NULL);
	}
	return (joined);
}
