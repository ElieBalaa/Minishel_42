/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:55:01 by oiskanda          #+#    #+#             */
/*   Updated: 2025/05/22 15:00:55 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_int_len(int n)
{
	int	len;

	if (n <= 0)
		len = 1;
	else
		len = 0;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	long	nb;
	char	*res;

	nb = n;
	len = ft_int_len(n);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res[len--] = '\0';
	if (nb == 0)
		res[0] = '0';
	if (nb < 0)
	{
		res[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		res[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (res);
}
