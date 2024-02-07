NAME = minishell
CC = gcc
FLAGS = -lreadline #-wall -wextra -werror
SRC = main.c\
	free_n_exit.c\
	prompt.c\
	env.c\
	quote.c\
	builtins/cd.c\
	builtins/pwd.c\
	builtins/echo.c
LIBFT = libft/libft.a
OBJ = $(SRC:c=o)

all : $(NAME)

$(NAME) : $(OBJ)
	make all -C libft
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT)

clean :
	make clean -C libft
	rm -rf *.o
fclean : clean
	make fclean -C libft
	rm -rf $(NAME)

re : fclean all