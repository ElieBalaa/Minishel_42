/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:13:36 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:14:09 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_key(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		++i;
	}
	return (1);
}

int	env_count(char **e)
{
	int	i;

	i = 0;
	while (e && e[i])
		++i;
	return (i);
}

void	sort_strings(char **arr)
{
	int		n;
	int		i;
	int		j;
	char	*tmp;

	n = env_count(arr);
	i = 0;
	while (i < n - 1)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	put_err(const char *p, const char *msg)
{
	write(2, "cd: ", 4);
	if (p && *p)
	{
		write(2, p, ft_strlen(p));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

int	chdir_error(const char *p)
{
	struct stat	st;

	if (stat(p, &st) != 0)
		return (put_err(p, "No such file or directory"), 1);
	if (!S_ISDIR(st.st_mode))
		return (put_err(p, "Not a directory"), 1);
	if (access(p, X_OK) != 0)
		return (put_err(p, "Permission denied"), 1);
	return (put_err(p, "error"), 1);
}
