NAME = minishell
CC = gcc
FLAGS = -lreadline #-wall -wextra -werror
SRC = main.c\
	cd.c\
	free_n_exit.c\
	pwd.c
LIBFT = libft/libft.a
OBJ = $(SRC:c=o)

all : $(NAME)

$(NAME) : $(OBJ)
	make all -C libft
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT)

clean :
	make clean -C libft
	rm main.o\
	cd.o\
	free_n_exit.o

fclean :
	make fclean -C libft
	rm -rf $(NAME)

re : fclean all