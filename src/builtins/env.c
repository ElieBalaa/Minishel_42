/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:16:09 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 19:00:41 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_var(t_minishell *sh, const char *key)
{
	size_t	key_len;
	int		i;

	if (!sh || !key || !sh->env)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (sh->env[i])
	{
		if (!ft_strncmp(sh->env[i], key, key_len) && sh->env[i][key_len] == '=')
			return (sh->env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

char	**env_copy(char **env)
{
	char	**copy;
	int		i;
	int		count;

	if (!env)
		return (NULL);
	count = env_count(env);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (env[i])
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			free_env_strings(copy);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	free_env_strings(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
}

int	builtin_env(t_minishell *sh, char **argv)
{
	char	**env;
	int		i;
	int		ignore_env;

	i = 1;
	ignore_env = 0;
	env = sh->env;
	while (argv[i] && argv[i][0] == '-')
	{
		if (!ft_strcmp(argv[i], "-i"))
			ignore_env = 1;
		else
			return (put_err("env", "invalid option"), 1);
		i++;
	}
	if (ignore_env)
		env = create_clean_env(sh);
	while (argv[i] && ft_strchr(argv[i], '='))
	{
		env = add_env_var(sh, env, argv[i]);
		i++;
	}
	if (!argv[i])
		return (print_env(env));
	return (execute_command(sh, env, &argv[i]));
}
