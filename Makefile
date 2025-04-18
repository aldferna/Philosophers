NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address #-fsanitize=thread
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


#limitar a 200 philos? mas de 0 excepto numero de comidas?



#— DRD: Run with valgrind --tool=drd to check for data races.

#— Helgrind: Run with valgrind --tool=helgrind to find lock issues and potential deadlocks.

#— FSanitise: Compile with -fsanitize=thread to detect threading problems while running.