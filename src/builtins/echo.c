/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:16:07 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/13 18:05:11 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parse_echo_flags(char **av, int *nl, int *i)
{
	int	j;

	while (av[*i] && av[*i][0] == '-' && av[*i][1] == 'n')
	{
		j = 2;
		while (av[*i][j] == 'n')
			j++;
		if (av[*i][j] == '\0')
		{
			*nl = 0;
			(*i)++;
		}
		else
			break ;
	}
}

int	builtin_echo(t_minishell *sh, char **av)
{
	int	i;
	int	nl;

	(void)sh;
	nl = 1;
	i = 1;
	parse_echo_flags(av, &nl, &i);
	while (av[i])
	{
		printf("%s", av[i]);
		if (av[i + 1])
			printf(" ");
		++i;
	}
	if (nl)
		printf("\n");
	return (0);
}
