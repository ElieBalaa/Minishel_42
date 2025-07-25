/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oiskanda <oiskanda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:13:07 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/25 21:13:12 by oiskanda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

typedef struct s_token
{
	char				*text;
	struct s_token		*next;
}	t_token;

typedef struct s_ast
{
	char				**cmd;
	char				*input;
	char				*output;
	char				*heredoc_delim;
	int					append;
	int					is_heredoc;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

#endif 