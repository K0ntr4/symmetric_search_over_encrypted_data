SRC = ./src/*.c
OBJ = $(SRC:.c=.o)
NAME = ssoed.out
CFLAGS = -Wall -g3 -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lsodium

INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib

all: $(NAME)

run_all: fclean format lint $(NAME)

$(NAME): $(OBJ)
	gcc $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

lint-fix:
	clang-tidy -p=./ --fix $(SRC)

lint:
	clang-tidy -p=./ $(SRC)

format:
	clang-format -i $(SRC)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all lint format clean fclean re run_all
