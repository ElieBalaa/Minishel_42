/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:27:02 by oiskanda          #+#    #+#             */
/*   Updated: 2025/05/22 15:01:43 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	chr_in_set(const char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*new_str;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = ft_strlen(s1);
	while (s1[i] && chr_in_set(s1[i], set))
		i++;
	while (j > i && chr_in_set(s1[j - 1], set))
		j--;
	len = j - i;
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, s1 + i, len);
	new_str[len] = '\0';
	return (new_str);
}
