# Project
NAME      := minishell

# Colors
RESET     := \033[0m
BOLD      := \033[1m
GREEN     := \033[32m
BLUE      := \033[34m
CYAN      := \033[36m
YELLOW    := \033[33m
RED       := \033[31m
MAGENTA   := \033[35m

# Compiler
CC        := cc
CFLAGS    := -Wall -Wextra -Werror -g
LDFLAGS   := -L$(HOME)/.brew/opt/readline/lib
CPPFLAGS  := -Iincludes -Ilibft -I$(HOME)/.brew/opt/readline/include
LDLIBS    := -lreadline -lhistory -Llibft -lft

# Sources
SRCS      := main.c \
			 src/pipes_redirec/pipes_utils.c \
			 src/pipes_redirec/pipes.c \
			 src/pipes_redirec/pipeline_utils.c \
			 src/pipes_redirec/pipe_helpers.c \
			 src/pipes_redirec/redirec_utils.c \
			 src/pipes_redirec/redirections.c \
             src/expansion/expansion.c \
             src/expansion/expansion_heredoc.c \
             src/expansion/expansion_helpers.c \
             src/expansion/expansion_utils.c \
             src/tokenizer/parser.c \
             src/tokenizer/utils.c \
             src/tokenizer/clean_up.c \
             src/tokenizer/word_splitting.c \
             src/lexer/lexer.c \
             src/lexer/lexer_helpers.c \
             src/lexer/lexer_token_utils.c \
             src/tokenizer/escape_utils.c \
             src/tokenizer/token.c \
             src/tokenizer/token_utils.c \
             src/tokenizer/token_helpers.c \
             src/tokenizer/pipeline_helpers.c \
             src/tokenizer/token_heredoc.c \
             src/tokenizer/token_heredoc_helpers.c \
             src/tokenizer/token_redir.c \
			 src/builtins/builtin_utils.c \
			 src/builtins/cd.c \
			 src/builtins/echo.c \
			 src/builtins/env.c \
			 src/builtins/env_utils.c \
			 src/builtins/exit.c \
			 src/builtins/unset.c \
			 src/builtins/export.c \
			 src/builtins/export_helpers.c \
			 src/builtins/pwd.c \
			 src/builtins/execute_builtin.c \
             src/gc/simple_gc.c \
             src/gc/gc_cleanup.c \
             src/gc/gc_malloc.c \
             src/gc/gc_tokenizer.c \
             src/gc/gc_whitespace.c \
             src/execution/executor.c \
             src/execution/executor_redirect.c \
             src/heredoc/heredoc.c \
             src/heredoc/heredoc_utils.c \
             src/heredoc/heredoc_extra.c \
             src/heredoc/heredoc_buffer.c \
             src/heredoc/heredoc_piped.c \
             src/execution/path_resolver.c \
             src/utils/shell_utils.c \
             src/utils/logical_operators.c \
             src/utils/logical_helpers.c \
             src/utils/init_shell.c \
             src/utils/env_init.c \
             src/utils/cleanup_shell.c \
             src/utils/utils.c \
             src/utils/get_next_line.c

# Objects
OBJ_DIR   := obj
OBJS      := $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Libft
LIBFT_DIR := libft
LIBFT_LIB := $(LIBFT_DIR)/libft.a

# Rules
.PHONY: all clean fclean re leaks norm

all: $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR) --silent

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS) $(LIBFT_LIB)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)
	@printf "\n$(GREEN)$(BOLD)$(NAME) compiled successfully!$(RESET)\n"
	@printf "$(GREEN)$(BOLD)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n"
	@printf "$(GREEN)$(BOLD)Ready to execute: ./$(NAME)$(RESET)\n"
	@printf "$(GREEN)$(BOLD)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n\n"

clean:
	@printf "$(YELLOW)Cleaning object files...$(RESET)\n"
	@rm -rf $(OBJ_DIR) 2>/dev/null || true
	@$(MAKE) -C $(LIBFT_DIR) clean --silent
	@printf "$(GREEN)$(BOLD)Clean completed successfully!$(RESET)\n\n"

fclean: clean
	@printf "$(RED)Removing $(NAME) executable...$(RESET)\n"
	@rm -f $(NAME) 2>/dev/null || true
	@$(MAKE) -C $(LIBFT_DIR) fclean --silent
	@printf "$(GREEN)$(BOLD)Full clean completed successfully!$(RESET)\n\n"

leaks: $(NAME)
	@printf "$(CYAN)$(BOLD)Running memory leak detection...$(RESET)\n"
	@printf "$(CYAN)$(BOLD)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n"
	@valgrind --leak-check=full -q ./$(NAME) 2>&1 | tee leak_output.log
	@printf "$(CYAN)$(BOLD)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n"


re: fclean all

# Prevent make from interpreting color codes as files
.SILENT:
