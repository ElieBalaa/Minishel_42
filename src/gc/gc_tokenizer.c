/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 00:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:16:11 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_tokensize(t_token *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

char	*qc_remove_quotes(const char *s)
{
	int		len;
	char	*out;
	int		i;
	int		j;

	if (!s)
		return (ft_strdup(""));
	len = ft_strlen(s);
	if (len < 2)
		return (ft_strdup(s));
	if (!((s[0] == '"' && s[len - 1] == '"')
			|| (s[0] == '\'' && s[len - 1] == '\'')))
		return (ft_strdup(s));
	out = malloc(len - 1);
	if (!out)
		return (NULL);
	i = 1;
	j = 0;
	while (i < len - 1)
		out[j++] = s[i++];
	out[j] = '\0';
	return (out);
}
