/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:30:00 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/23 21:16:48 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_quote_or_empty_case(t_minishell *sh, int *adv)
{
	*adv = 0;
	return (gc_strdup(sh, "$"));
}

static char	*handle_invalid_var_case(t_minishell *sh, const char *p, int *adv)
{
	char	*temp_val;

	*adv = 1;
	temp_val = gc_malloc(sh, 3);
	if (temp_val)
	{
		temp_val[0] = '$';
		temp_val[1] = *p;
		temp_val[2] = '\0';
	}
	return (temp_val);
}

static char	*handle_exit_status_case(t_minishell *sh, int *adv)
{
	*adv = 1;
	return (gc_strdup(sh, "$$EXIT_STATUS$$"));
}

static char	*handle_special_cases(t_minishell *sh, const char *p, int *adv)
{
	if (*p == '"' || *p == '\0')
		return (handle_quote_or_empty_case(sh, adv));
	if (*p == '?')
		return (handle_exit_status_case(sh, adv));
	if (ft_isdigit(*p))
	{
		*adv = 1;
		return (gc_strdup(sh, ""));
	}
	if (!ft_isalpha(*p) && *p != '_')
		return (handle_invalid_var_case(sh, p, adv));
	return (NULL);
}

char	*expand_one(t_minishell *sh, const char *p, int *adv)
{
	char	*val;

	val = handle_special_cases(sh, p, adv);
	if (val != NULL)
		return (val);
	*adv = 1;
	while (p[*adv] && (ft_isalnum(p[*adv]) || p[*adv] == '_'))
		(*adv)++;
	return (lookup_env_var(sh, p, *adv));
}
