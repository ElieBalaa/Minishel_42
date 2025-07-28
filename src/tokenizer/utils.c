/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:00:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 15:06:10 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redir(char *s)
{
	return (ft_strcmp(s, "<") == 0 || ft_strcmp(s, ">") == 0
		|| ft_strcmp(s, ">>") == 0 || ft_strcmp(s, "<<") == 0);
}

int	count_args(char **tok, int n)
{
	int	i;
	int	cnt;

	if (!tok || n <= 0)
		return (0);
	i = 0;
	cnt = 0;
	while (i < n && tok[i])
	{
		if (is_semicolon_error(tok[i]))
			return (-1);
		if (is_redir(tok[i]))
		{
			if (i + 1 >= n || !tok[i + 1] || is_redir(tok[i + 1]))
				return (-1);
			i += 2;
		}
		else
		{
			cnt++;
			i++;
		}
	}
	return (cnt);
}

int	quotes_balanced(const char *s)
{
	int	in_single;
	int	in_double;
	int	i;

	if (!s)
		return (1);
	in_single = 0;
	in_double = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !in_double)
			in_single ^= 1;
		else if (s[i] == '"' && !in_single)
			in_double ^= 1;
		else if (s[i] == '\\' && s[i + 1] && !in_single)
			++i;
		++i;
	}
	if (!in_single && !in_double)
		return (1);
	return (0);
}

char	*strip_surrounding_quotes(const char *str)
{
	int		len;
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''))
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		i = 0;
		while (i < len - 2)
		{
			result[i] = str[i + 1];
			i++;
		}
		result[len - 2] = '\0';
		return (result);
	}
	return (ft_strdup(str));
}

int	is_pipeline_end(char **w, int i)
{
	return (ft_strcmp(w[i], "|") == 0 || w[i + 1] == NULL);
}
