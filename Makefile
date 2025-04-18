NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRC = main.c utils.c utils_philo.c philo_routines.c\

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
