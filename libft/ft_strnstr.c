/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:36:26 by oiskanda          #+#    #+#             */
/*   Updated: 2025/05/22 15:01:30 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	little_len;

	i = 0;
	if (!big && len == 0)
		return (NULL);
	little_len = ft_strlen(little);
	if (little_len == 0)
		return ((char *)big);
	while (i + little_len <= len && big[i])
	{
		if (ft_strncmp(&big[i], little, little_len) == 0)
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
