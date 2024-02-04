NAME = minishell
CC = gcc
FLAGS = -lreadline #-wall -wextra -werror
SRC = main.c\
		free_n_exit.c
LIBFT = libft/libft.a
OBJ = $(SRC:c=o)

all : $(NAME)

$(NAME) : $(OBJ)
	make all -C libft
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT)

fclean :
	make fclean -C libft
	rm -rf $(NAME)

re : fclean all