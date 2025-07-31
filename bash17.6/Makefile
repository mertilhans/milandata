NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRC = shell.c lexer.c lexer_extension.c lexer_utils.c free.c utils.c parser.c execute.c builtin.c
 
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
