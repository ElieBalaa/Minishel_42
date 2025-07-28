/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/27 18:46:55 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/stat.h>

static char	*join_path_and_cmd(const char *path, const char *cmd)
{
	char	*full_path;
	size_t	path_len;
	size_t	cmd_len;

	path_len = ft_strlen(path);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(path_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_memcpy(full_path, path, path_len);
	full_path[path_len] = '/';
	ft_memcpy(full_path + path_len + 1, cmd, cmd_len);
	full_path[path_len + cmd_len + 1] = '\0';
	return (full_path);
}

static int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR));
}

static char	*search_in_path_dirs(char **path_dirs, const char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = join_path_and_cmd(path_dirs[i], cmd);
		if (full_path && is_executable(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	*get_path_directories(t_minishell *sh)
{
	char	*path_var;

	path_var = get_env_var(sh, "PATH");
	if (!path_var)
		return (NULL);
	return (path_var);
}

char	*resolve_command_path(t_minishell *sh, const char *cmd)
{
	char	*path_var;
	char	**path_dirs;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_path_directories(sh);
	if (!path_var)
		return (NULL);
	path_dirs = ft_split(path_var, ':');
	if (!path_dirs)
		return (NULL);
	full_path = search_in_path_dirs(path_dirs, cmd);
	ft_free_split(path_dirs);
	return (full_path);
}
