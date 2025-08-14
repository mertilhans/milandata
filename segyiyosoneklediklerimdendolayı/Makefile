NAME		=	minishell
CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror
LDFLAGS		=	-lreadline

RED			=	\033[0;31m
GREEN		=	\033[0;32m
YELLOW		=	\033[1;33m
BLUE		=	\033[0;34m
NC			=	\033[0m

SRC			=	main.c process_line.c shell_init.c exit_status.c \
				lexer.c lexer_utils.c lexer_extension.c tokenizer_utils.c \
				parser_main.c parser_cmd.c parser_loop.c parser_redir.c parser_expand.c parser_utils.c \
				expander_main.c expander_dollar.c expander_builder.c expander_legacy.c \
				exec_main.c exec_process.c exec_child.c exec_builtin.c exec_redir.c exec_data.c exec_utils.c \
				heredoc_main.c heredoc_read.c heredoc_loop.c heredoc_utils.c \
				builtin_cd.c builtin_echo.c builtin_pwd.c builtin_env.c builtin_export.c builtin_unset.c builtin_exit.c builtin_utils.c \
				env_init.c env_operations.c env_basic.c env_array.c env_free.c \
				export_basic.c export_operations.c export_process.c export_utils.c \
				signal_handler.c signal_utils.c \
				gb_main.c gb_basic.c gb_env.c \
				path_utils.c utils.c utils_convert.c utils_memory.c utils_str.c utils_str_extra.c

OBJ			=	$(SRC:.c=.o)

all:			$(NAME)

$(NAME):		$(OBJ)
				@echo "$(YELLOW)🔗 Nesne dosyaları bağlanıyor...$(NC)"
				@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
				@echo "$(GREEN)✅ $(NAME) başarıyla derlendi!$(NC)"

%.o:			%.c shell.h
				@echo "$(BLUE)🔨 Derleniyor: $<...$(NC)"
				@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
				@echo "$(YELLOW)🧹 Nesne dosyaları temizleniyor...$(NC)"
				@rm -f $(OBJ)
				@echo "$(GREEN)✅ Nesne dosyaları temizlendi!$(NC)"

fclean:			clean
				@echo "$(YELLOW)🔥 Çalıştırılabilir dosya kaldırılıyor...$(NC)"
				@rm -f $(NAME)
				@echo "$(GREEN)✅ Tam temizlik tamamlandı!$(NC)"

re:				fclean all

.PHONY:			all clean fclean re