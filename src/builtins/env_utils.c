/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:30:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 17:30:00 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_putendl_fd(env[i], 1);
		++i;
	}
	return (0);
}

char	**create_clean_env(t_minishell *sh)
{
	char	**clean_env;

	clean_env = gc_malloc(sh, sizeof(char *) * 1);
	if (!clean_env)
		return (NULL);
	clean_env[0] = NULL;
	return (clean_env);
}

char	**add_env_var(t_minishell *sh, char **env, const char *var)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (env[count])
		count++;
	new_env = gc_malloc(sh, sizeof(char *) * (count + 2));
	if (!new_env)
		return (env);
	i = 0;
	while (i < count)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[count] = gc_strdup(sh, var);
	new_env[count + 1] = NULL;
	return (new_env);
}

static char	**set_underscore_var(t_minishell *sh, char **env,
			char **cmd, char *path)
{
	char	*underscore_entry;
	char	*abs_path;

	if (cmd[0][0] == '.' && cmd[0][1] == '/')
	{
		abs_path = realpath(cmd[0], NULL);
		if (abs_path)
		{
			underscore_entry = gc_strjoin(sh, "_=", abs_path);
			free(abs_path);
		}
		else
			underscore_entry = gc_strjoin(sh, "_=", path);
	}
	else
		underscore_entry = gc_strjoin(sh, "_=", path);
	return (add_env_var(sh, env, underscore_entry));
}

int	execute_command(t_minishell *sh, char **env, char **cmd)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		path = resolve_command_path(sh, cmd[0]);
		if (path)
		{
			env = set_underscore_var(sh, env, cmd, path);
			execve(path, cmd, env);
		}
		write(2, "env: ", 5);
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, ": No such file or directory\n", 28);
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
