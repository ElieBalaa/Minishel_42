/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:14:53 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_builtin(char *s)
{
	return (!ft_strcmp(s, "echo") || !ft_strcmp(s, "cd")
		|| !ft_strcmp(s, "pwd") || !ft_strcmp(s, "export")
		|| !ft_strcmp(s, "unset") || !ft_strcmp(s, "env")
		|| !ft_strcmp(s, "exit"));
}

int	classify_exec_error(const char *path)
{
	struct stat	st;

	if (!path || access(path, F_OK) != 0)
		return (127);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (126);
	if (access(path, X_OK) != 0)
		return (126);
	return (126);
}

void	exec_node(t_minishell *sh, t_ast *node)
{
	char	*path;
	int		code;

	if (!node->cmd || !node->cmd[0])
		exit(0);
	if (is_builtin(node->cmd[0]))
		exit(execute_builtin(sh, node->cmd));
	path = resolve_command_path(sh, node->cmd[0]);
	if (path)
		execve(path, node->cmd, sh->env);
	code = classify_exec_error(path);
	write(2, "minishell: ", 11);
	write(2, node->cmd[0], ft_strlen(node->cmd[0]));
	if (code == 127)
		write(2, ": command not found\n", 20);
	else if (path && access(path, X_OK) != 0)
		write(2, ": Permission denied\n", 20);
	else
		write(2, ": Is a directory\n", 17);
	exit(code);
}
