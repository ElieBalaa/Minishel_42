/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:07:38 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 14:23:54 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	print_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

static char	*get_export_key(t_minishell *sh, const char *arg)
{
	size_t	len;
	char	*key;

	(void)sh;
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = malloc(len + 1);
	if (!key)
		return (NULL);
	ft_memcpy(key, arg, len);
	key[len] = '\0';
	return (key);
}

static int	is_valid_export_key(const char *key)
{
	size_t	i;

	if (!key[0] || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		++i;
	}
	return (1);
}

static int	perform_export(t_minishell *sh, const char *arg, const char *key)
{
	size_t	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		++i;
	if (arg[i] == '=')
		return (env_set(sh, arg));
	i = 0;
	while (sh->env[i])
		++i;
	return (extend_env_array(sh, key, i));
}

int	process_export_arg(t_minishell *sh, char *arg)
{
	char	*key;
	int		err;

	key = get_export_key(sh, arg);
	if (!key)
		return (1);
	if (!is_valid_export_key(key))
		return (free(key), print_error(arg));
	err = perform_export(sh, arg, key);
	free(key);
	return (err);
}
