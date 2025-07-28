/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:16:16 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 14:05:14 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_cd_error(t_minishell *sh, char *dst, char *old, int print)
{
	if (print)
		ft_putendl_fd(dst, 1);
	update_env(sh, old);
	return (0);
}

int	builtin_pwd(t_minishell *sh, char **av)
{
	char	*cwd;

	(void)av;
	(void)sh;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup(get_env_var(sh, "PWD"));
	if (!cwd)
		return (perror("minishell: pwd"), 1);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
