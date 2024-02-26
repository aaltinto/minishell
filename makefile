NAME = minishell
CC = gcc
FLAGS = -Wall -Wextra -Werror -lreadline -g
SRC = main.c\
	free_n_exit.c\
	prompt.c\
	env.c\
	utils.c\
	quote.c\
	denem.c\
	parse.c\
	parse_utils.c\
	builtins/cd.c\
	builtins/cmd_env.c\
	builtins/export.c\
	builtins/pwd.c\
	builtins/echo.c

LIBFT = libft/libft.a
GNL = gnl/gnl.a
OBJ = $(SRC:c=o)

all : $(NAME)

$(NAME) : $(OBJ)
	make all -C libft
	make all -C gnl
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(GNL)

clean :
	make clean -C libft
	make clean -C gnl
	rm -rf *.o
	rm -rf builtins/*.o
fclean : clean
	make fclean -C libft
	make fclean -C gnl
	rm -rf $(NAME)

re : fclean all