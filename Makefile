SRC = ./ssoed.c

OBJ = $(SRC:.c=.o)

NAME = ssoed

$(NAME):
	gcc $(SRC) -o $(NAME) -lsodium -Wall -g3;

all: $(NAME)

clean:
	rm -f $(OBJ);

fclean: clean
	rm -f $(NAME);

re: fclean all

.PHONY: all clean fclean re