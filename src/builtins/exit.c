/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:16:12 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:14:05 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	str_to_ll(const char *s, long long *out)
{
	unsigned long long	acc;
	int					sign;

	if (!s || !*s)
		return (0);
	sign = 1;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	if (!ft_isdigit(*s))
		return (0);
	acc = 0;
	while (*s && ft_isdigit(*s))
	{
		acc = acc * 10 + (*s - '0');
		++s;
	}
	if (*s || acc > 9223372036854775807ULL + (sign == -1))
		return (0);
	*out = (long long)(sign * acc);
	return (1);
}

static int	count_exit_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

static void	handle_exit_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

static void	process_exit_with_arg(char **av, int argc)
{
	long long	val;
	char		*arg;

	arg = strip_surrounding_quotes(av[1]);
	if (!arg)
		exit(1);
	if (!str_to_ll(arg, &val))
	{
		handle_exit_error(av[1]);
		free(arg);
		exit(2);
	}
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		free(arg);
		exit(1);
	}
	free(arg);
	exit((unsigned char)val);
}

int	builtin_exit(t_minishell *sh, char **av)
{
	int	argc;

	if (sh->is_interactive)
		ft_putendl_fd("exit", 1);
	argc = count_exit_args(av);
	if (argc == 1)
		exit(sh->last_exit);
	process_exit_with_arg(av, argc);
	return (1);
}
