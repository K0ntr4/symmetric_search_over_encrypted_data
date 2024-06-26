SRC = ./src/ssoed.c ./src/utils.c ./src/base_encryption.c ./src/basic_scheme.c ./src/controlled_scheme.c ./src/controlled_chapter_scheme.c ./src/controlled_hierarchical_scheme.c
OBJ = $(SRC:.c=.o)
NAME = ssoed.out
CFLAGS = -Wall -Werror -g3 -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lsodium

INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib

all: $(NAME)

run_all: fclean format lint $(NAME) clean

$(NAME): $(OBJ)
	gcc $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

lint-fix:
	clang-tidy -p=./ --fix $(SRC)

lint:
	clang-tidy -p=./ --warnings-as-errors=* $(SRC)

format:
	clang-format -i $(SRC)
	clang-format -i include/*.h

check-memory:
	valgrind --leak-check=full --track-origins=yes --show-error-list=yes -s ./ssoed.out

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all lint format clean fclean re run_all checkmemory
