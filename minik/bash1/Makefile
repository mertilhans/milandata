NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRC = \
	lexer/shell.c \
	lexer/lexer.c \
	lexer/lexer_extension.c \
	lexer/lexer_utils.c \
	lexer/utils.c \
	gb/gb.c \
	parser/parser.c \
	parser/parser_utils.c \
	parser/parser_extansion.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re