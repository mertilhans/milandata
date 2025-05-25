NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

SRC = sss.c
OBJ = sss.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

sss.o: sss.c
	$(CC) $(CFLAGS) -c sss.c -o sss.o

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
