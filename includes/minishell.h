/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eal-bala <eal-bala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 21:13:23 by oiskanda          #+#    #+#             */
/*   Updated: 2025/07/28 22:31:55 by eal-bala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stddef.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>

typedef struct s_pipe_ctx
{
	int		(*pipes)[2];
	int		count;
	pid_t	*pids;
}	t_pipe_ctx;

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_simple_gc
{
	t_gc_node			*head;
	int					count;
}	t_simple_gc;

typedef struct s_token
{
	char				*text;
	int					quoted;
	struct s_token		*next;
}	t_token;

typedef struct s_redir
{
	char			op;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	char				**cmd;
	char				*input;
	char				*output;
	int					heredoc_fd;
	char				**heredoc_delims;
	int					append;
	int					is_heredoc;
	t_redir				*redirs;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

typedef struct s_minishell
{
	char				**env;
	int					last_exit;
	int					is_interactive;
	int					child_pid;
	int					heredoc_active;
	int					cleaned_up;
	t_simple_gc			gc;
}	t_minishell;

typedef struct s_pipeline_ctx
{
	t_ast				**root;
	t_ast				**cur;
}	t_pipeline_ctx;

typedef struct s_escape_ctx
{
	int					*i;
	int					*j;
	char				*result;
	int					*in_single;
	int					*in_double;
}	t_escape_ctx;

typedef struct s_expand_ctx
{
	t_minishell			*sh;
	const char			*s;
	size_t				*i;
	int					*q;
	char				*last_result;
}	t_expand_ctx;

typedef struct s_fill_args
{
	char				**tokens;
	int					n;
	char				**argv;
	t_ast				*node;
	t_minishell			*sh;
}	t_fill_args;

typedef struct s_pipeline_args
{
	char				**w;
	int					start;
	int					i;
	t_minishell			*sh;
}	t_pipeline_args;

typedef struct s_child_args
{
	const char			*path;
	char				**argv;
	char				**envp;
	t_minishell			*sh;
	t_ast				*node;
}	t_child_args;

typedef struct s_input_vars
{
	char	*input;
	size_t	size;
	size_t	capacity;
}	t_input_vars;

typedef struct s_content_vars
{
	char	*content;
	size_t	size;
	size_t	capacity;
}	t_content_vars;

typedef struct s_heredoc_loop_params
{
	t_minishell		*sh;
	char			**delimiters;
	int				is_piped;
	int				last_index;
	t_content_vars	*vars;
}	t_heredoc_loop_params;

typedef struct s_delimiter_content_params
{
	t_minishell		*sh;
	char			*delimiter;
	int				is_piped;
	int				is_last;
	t_content_vars	*vars;
}	t_delimiter_content_params;

typedef struct s_content_processing_params
{
	t_minishell		*sh;
	char			*content;
	int				i;
	int				last_index;
	char			**concatenated_content;
	size_t			*total_size;
}	t_content_processing_params;

typedef struct s_loop_iteration_params
{
	t_minishell		*sh;
	char			**delimiters;
	int				last_index;
	char			*full_input;
	int				i;
	char			*content;
	char			*concatenated_content;
	size_t			total_size;
}	t_loop_iteration_params;

typedef struct s_heredoc_args
{
	t_minishell	*sh;
	char		*delimiter;
	int			*pipe_fd;
	int			is_piped;
}	t_heredoc_args;

typedef struct s_iteration_args
{
	t_minishell	*sh;
	char		*line;
	char		*clean_delim;
	int			*pipe_fd;
	int			is_quoted;
}	t_iteration_args;

typedef struct s_expansion
{
	size_t	i;
	int		in_single;
	int		in_double;
	char	*result;
}	t_expansion;

typedef struct s_child_params
{
	t_minishell	*sh;
	t_ast		*cmd;
	t_pipe_ctx	*c;
	int			i;
	int			n;
}	t_child_params;

typedef struct s_pipeline_params
{
	char		**words;
	int			*i;
	int			*start;
	t_ast		**node;
	t_minishell	*sh;
}	t_pipeline_params;

char		*handle_escaped_char_heredoc(t_minishell *sh,
				const char *s, t_expansion *exp);
int			validate_key(const char *key);
void		process_input_redir(char **tok,
				int *i, t_ast *node, t_minishell *sh);
int			process_export_arg(t_minishell *sh, char *arg);
int			extend_env_array(t_minishell *sh, const char *str, int i);
int			env_set(t_minishell *sh, const char *str);
void		fill_cmds(t_ast *n, t_ast **arr, int *idx);
int			count_cmds(t_ast *n);
int			flatten_pipeline(t_ast *root, t_ast ***out, t_minishell *sh);
char		*handle_char(t_minishell *sh, const char s, char *res);
char		*handle_dollar(t_minishell *sh,
				const char *s, size_t *i, char *res);
void		add_redir(t_ast *node, char op, char *file, t_minishell *sh);
void		repl(t_minishell *sh);
void		process_line(t_minishell *sh, char *line);
void		handle_quotes(char c, t_expansion *exp);
char		*handle_dollar_quote(t_minishell *sh,
				const char *s, t_expansion *exp);
char		*handle_ansi_c_quote(t_minishell *sh,
				const char *s, t_expansion *exp);
char		*lookup_env_var(t_minishell *sh, const char *name, int len);
char		*process_dollar(t_minishell *sh, const char *s, t_expansion *exp);
/* clean up */
void		ft_free_split(char **str);
void		free_tokens(t_token **tokens);
void		free_ast(t_ast *node);
void		free_toklist(t_token *lst);
void		gc_free_nodes(t_minishell *sh);

/* tokenization */
t_token		**split_on_space(char *str);
t_token		*tok_last(t_token *lst);
t_token		*tokenize(const char *line, t_minishell *sh);
char		**tok_to_array(t_minishell *sh, t_token *lst);
const char	*skip_quotes(const char *p);

/* parsing */
t_ast		*parse_pipeline(char **words, t_minishell *sh);
t_ast		*parse_segment(char **tokens, int n, t_minishell *sh);
t_ast		*parse_line(const char *line, t_minishell *sh);
t_ast		*init_ast_node(t_minishell *sh);
int			is_pipeline_end(char **w, int i);
t_ast		*process_pipeline_segment(t_pipeline_params params);
int			count_till_pipe(char **words);
t_ast		*handle_pipe_error(t_minishell *sh);

/* expansion */
char		*expand_vars(t_minishell *sh, const char *s);
char		*expand_vars_heredoc(t_minishell *sh, const char *s);
char		*lookup_env_var(t_minishell *sh, const char *name, int len);
char		*expand_one(t_minishell *sh, const char *p, int *adv);
char		*handle_dollar(t_minishell *sh, const char *s,
				size_t *i, char *res);
char		*handle_char(t_minishell *sh, const char s, char *res);

/* escape_utils */
int			should_process_escape(char next_char, int in_single, int in_double);
char		*process_token_escapes(t_minishell *sh, const char *str);

/* execution */
int			execute_ast(t_minishell *sh, t_ast *ast);
char		*resolve_command_path(t_minishell *sh, const char *cmd);
int			fork_and_execute(t_minishell *sh, const char *path,
				char **argv, t_ast *node);
int			setup_input_redirect(t_minishell *sh, t_ast *node);
int			validate_pipeline(t_ast *pipeline, t_ast ***cmds, t_minishell *sh);
int			setup_pipeline_context(t_pipe_ctx *c, int n, t_ast **cmds);
int			handle_pipeline_error(t_ast **cmds, t_pipe_ctx *c);

/* heredoc */
int			process_heredoc(t_minishell *sh, char *delimiter);
int			process_multiple_heredocs(t_minishell *sh, char **delimiters,
				int *pipe_fd);
char		*read_heredoc_line(int is_piped);
int			check_delimiter_match(char *line, char *delimiter);
int			handle_heredoc_loop(t_minishell *sh, char *delimiter,
				int *pipe_fd, int is_piped);
int			process_piped_content(char **content, char **current_pos,
				char *delimiter);

/* builtins */
int			update_env(t_minishell *sh, const char *old);
int			handle_cd_error(t_minishell *sh, char *dst, char *old, int print);
int			chdir_error(const char *p);
void		put_err(const char *p, const char *msg);
int			builtin_exit(t_minishell *sh, char **args);
int			builtin_echo(t_minishell *sh, char **args);
int			builtin_cd(t_minishell *sh, char **args);
int			builtin_pwd(t_minishell *sh, char **args);
int			builtin_env(t_minishell *sh, char **argv);
int			builtin_export(t_minishell *sh, char **args);
int			builtin_unset(t_minishell *sh, char **args);
int			execute_builtin(t_minishell *sh, char **args);

/* env helpers */
int			is_valid_key(const char *s);
int			env_count(char **e);
void		sort_strings(char **arr);
char		*get_env_var(t_minishell *sh, const char *key);
char		**env_copy(char **env);
void		free_env_strings(char **env);
int			env_set(t_minishell *sh, const char *str);
void		env_unset(t_minishell *sh, const char *key);

/* env utilities */
int			print_env(char **env);
char		**create_clean_env(t_minishell *sh);
char		**add_env_var(t_minishell *sh, char **env, const char *var);
int			execute_command(t_minishell *sh, char **env, char **cmd);

/* utils */
int			is_semicolon_error(char *token);
void		shlvl(t_minishell *sh);
void		set_initial_env_vars(t_minishell *sh);
void		copy_env_vars(t_minishell *sh, char **envp, size_t n);
char		*get_colored_prompt(t_minishell *sh);
int			is_quoted(char *str);
int			is_operator(char c);
int			is_space(char c);
int			is_redir(char *s);
int			count_args(char **tok, int n);
int			op_len(const char *s);
int			quotes_balanced(const char *s);
char		*strip_surrounding_quotes(const char *str);
int			process_redir(char **tok, int *i, t_ast *node, t_minishell *sh);
int			ft_tokensize(t_token *lst);
void		sig_handler(int sig);
void		heredoc_sig_handler(int sig);
int			status_code(int wstatus);
void		init_minishell(t_minishell *sh, char **envp);
void		cleanup_shell(t_minishell *sh);
void		*ft_realloc(void *ptr, size_t new_size);
void		*ft_memcpy(void *dest, const void *src, size_t n);
char		*process_quoted_delimiter(t_minishell *sh,
				const char *start, int len);
int			process_logical_operators(t_minishell *sh, char *line);
char		*find_or_operator(char *line, int *is_or);

/* garbage collector */
int			gc_init(t_minishell *sh);
void		gc_cleanup_all(t_minishell *sh);
void		gc_free_nodes(t_minishell *sh);
void		*gc_malloc(t_minishell *sh, size_t size);
char		*gc_strdup(t_minishell *sh, const char *s);
char		*gc_strndup(t_minishell *sh, const char *s, int n);
int			gc_add(t_minishell *sh, void *ptr);
void		*gc_realloc(t_minishell *sh, void *ptr,
				size_t old_size, size_t new_size);
t_token		**gc_split_on_space(t_minishell *sh, char *str);
int			is_whitespace(char c);
int			count_words(char *str);
char		*extract_word(char *str, int *start);
char		*gc_strjoin(t_minishell *sh, const char *s1, const char *s2);
char		*qc_remove_quotes(const char *s);
char		**split_whitespace(char *str);
void		shlvl(t_minishell *sh);

/* Heredoc related function prototypes */
void		heredoc_prompt(void);
int			append_line_to_input(t_input_vars *vars, char *line);
char		*extract_line(char **current_pos);
int			append_content_line(char **content, char *line,
				size_t *content_size, size_t *content_capacity);
void		finalize_content(char **content, char **temp_content,
				size_t *temp_size);
int			append_temp_content(char **temp_content, char *line,
				size_t *temp_size, size_t *temp_capacity);
int			find_last_delimiter_index(char **delimiters);
int			process_content(t_minishell *sh, char **delimiters, int *pipe_fd,
				int *quoted_flags);
int			process_heredoc(t_minishell *sh, char *delimiter);
int			resize_content_buffer(t_content_vars *vars);
int			append_expanded_line(t_minishell *sh, char *line,
				t_content_vars *vars);
int			append_raw_line(t_minishell *sh, char *line,
				t_content_vars *vars);
void		init_content_vars(t_content_vars *vars);
int			process_delimiter_content(t_delimiter_content_params *params);
int			init_heredoc_vars(char **delimiters, int *i,
				int *last_index, int *is_piped);
int			process_multiple_heredocs(t_minishell *sh, char **delimiters,
				int *pipe_fd);

/* token_heredoc.c */
void		process_heredoc_redir(t_minishell *sh,
				char **tok, int *i, t_ast *node);
char		*remove_quotes(char *str);

/* get_next_line.c */
char		*get_next_line(int fd);

/* lexer helpers */
void		advance_word(const char **ptr);
void		process_operator(t_token **lst, t_minishell *sh, const char **p);
void		process_word(t_token **lst, t_minishell *sh, const char **p);
void		add_tok(t_token **lst, t_minishell *sh, const char *start, int len);

/* token_heredoc helpers */
void		copy_existing_delims(char **new_delims,
				char **heredoc_delims, int count);
char		*prepare_quoted_delimiter(char *delimiter);
void		add_heredoc_delimiter(t_ast *node, char *delimiter);
int			is_quoted_delimiter(char *str);
int			is_originally_quoted(const char *delimiter);
void		sig_handler(int sig);
void		heredoc_sig_handler(int sig);

/* ─────────────────────────  execution layer  ─────────────────────────── */
void		exec_node(t_minishell *sh, t_ast *node);
int			fork_and_execute(t_minishell *sh,
				const char *path, char **argv, t_ast *node);
int			execute_pipeline(t_minishell *sh, t_ast *pipeline);
int			execute_ast(t_minishell *sh, t_ast *ast);

/* ─────────────────────────  pipe helpers  ────────────────────────────── */
int			alloc_pipes(t_pipe_ctx *ctx, int nb_pairs);
void		close_all_pipes(t_pipe_ctx *ctx);
void		free_pipes(t_pipe_ctx *ctx);
void		dup_pipes_child(t_pipe_ctx *ctx, int idx, int cmdcnt);
int			wait_pipeline(pid_t *pids, int cmdcnt);

/* ─────────────────────────  heredoc pre-pass  ────────────────────────── */
int			open_heredoc(char *d, int interactive, int *out_fd);
int			preprocess_heredocs(t_ast **cmds, int n, t_minishell *sh);

/* ─────────────────────────  redirections  ────────────────────────────── */
int			redirect_input(const char *file);
int			redirect_output(const char *file, int append);
int			apply_redirections(t_ast *node); /* << < > >> */
int			open_all_heredocs(t_ast *node, t_minishell *sh);

/* heredoc_utils.c */
int			is_quoted_delimiter(char *delimiter);
char		*get_clean_delimiter(char *delimiter);
int			heredoc_expand_variables(t_minishell *sh, char **content);

void		ignore_signals(void);
void		setup_signals(void);

/* word splitting */
t_token		*split_unquoted_token(t_minishell *sh, t_token *token);
t_token		*apply_word_splitting(t_minishell *sh, t_token *tokens);

/* lexer token utils */
char		*process_token_content(t_minishell *sh, const char *start,
				int len, int is_heredoc_delim);

#endif
