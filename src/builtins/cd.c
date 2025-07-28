/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:16:05 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 14:12:55 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*dst_path(t_minishell *sh, char **av, int *print)
{
	if (!av[1] || !ft_strcmp(av[1], "~") || !ft_strcmp(av[1], "--"))
		return (get_env_var(sh, "HOME"));
	if (!ft_strcmp(av[1], "-"))
	{
		*print = 1;
		return (get_env_var(sh, "OLDPWD"));
	}
	if (av[1][0] == '-' && av[1][1])
		return (put_err(av[1], "invalid option"), NULL);
	return (av[1]);
}

int	update_env(t_minishell *sh, const char *old)
{
	char	buf[1024];
	char	*tmp_old_pwd;
	char	*tmp_pwd;

	if (getcwd(buf, sizeof(buf)) == NULL)
		return (put_err(NULL, "error retrieving current directory 3"), 0);
	if (get_env_var(sh, "OLDPWD") != NULL)
	{
		tmp_old_pwd = gc_strjoin(sh, "OLDPWD=", old);
		env_set(sh, tmp_old_pwd);
	}
	if (get_env_var(sh, "PWD") != NULL)
	{
		tmp_pwd = gc_strjoin(sh, "PWD=", buf);
		env_set(sh, tmp_pwd);
	}
	return (1);
}

static int	climb_dead_tree(t_minishell *sh, char **dst)
{
	char	*cur;
	char	*slash;

	cur = get_env_var(sh, "PWD");
	if (!cur || !*cur)
		return (1);
	cur = gc_strdup(sh, cur);
	while (1)
	{
		if (chdir(cur) == 0)
		{
			*dst = cur;
			return (0);
		}
		slash = ft_strrchr(cur, '/');
		if (!slash)
			break ;
		if (slash == cur)
		{
			cur[1] = '\0';
			continue ;
		}
		*slash = '\0';
	}
	return (1);
}

static int	change_directory(t_minishell *sh,
	char **av, char *old, char *fallback)
{
	char	*dst;
	char	*abs;
	int		print;

	print = 0;
	dst = dst_path(sh, av, &print);
	if (!dst || !*dst)
		return (1);
	if (fallback && dst[0] != '/')
	{
		abs = gc_strjoin(sh, fallback, "/");
		abs = gc_strjoin(sh, abs, dst);
		if (abs && chdir(abs) == 0)
			dst = abs;
		else if (chdir(dst) != 0 && climb_dead_tree(sh, &dst))
			return (chdir_error(dst));
	}
	else if (chdir(dst) != 0)
		if (ft_strcmp(dst, "..") || climb_dead_tree(sh, &dst))
			return (chdir_error(dst));
	return (handle_cd_error(sh, dst, old, print));
}

int	builtin_cd(t_minishell *sh, char **av)
{
	char	old[1024];
	char	*fallback;

	fallback = NULL;
	if (av[1] && av[2])
		return (put_err(NULL, "too many arguments"), 1);
	if (!getcwd(old, sizeof old))
	{
		fallback = get_env_var(sh, "PWD");
		if (fallback)
			ft_strlcpy(old, fallback, sizeof old);
		put_err(NULL, "error retrieving current directory");
	}
	return (change_directory(sh, av, old, fallback));
}
