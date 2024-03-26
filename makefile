NAME = minishell
CC = gcc
FLAGS = -Wall -Wextra -Werror -g
SRC = main.c\
	free_n_exit.c\
	prompt.c\
	env.c\
	utils.c\
	quote.c\
	exec.c\
	pipe.c\
	open_create.c\
	parse.c\
	dollar_parse.c\
	signals.c\
	heredoc_append.c\
	input_output.c\
	parse_utils.c\
	builtins/cd.c\
	builtins/cmd_env.c\
	builtins/export_utils.c\
	builtins/export.c\
	builtins/unset.c\
	builtins/pwd.c\
	builtins/echo.c

LIBFT = libft/libft.a
OBJ = $(SRC:c=o)

GREEN := \033[0;32m
RED := \033[0;31m
RESET := \033[0m

BUILD_PRINT = @echo "$(GREEN)Building $@$(RESET)"
DELETE_PRINT = @echo "$(RED)Deleting ./minishell$(RESET)"
DELETE_OBJ = @echo "$(RED)Deleting Objects $(RESET)"
SUCCESS_MSG = @printf "$(GREEN)Compilation successful: $(NAME)$(RESET)\n"

all : $(NAME)

$(NAME) : $(OBJ)
	$(BUILD_PRINT)
	@make all -C libft
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	@printf "\033[K\r"
	$(SUCCESS_MSG)

%.o: %.c
	@printf "$(GREEN)Compiling $<$(RESET)\033[K\r"
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "\033[K\r"

clean :
	@make clean -C libft
	$(DELETE_OBJ)
	@rm -rf *.o
	@rm -rf builtins/*.o
fclean : clean
	@make fclean -C libft
	@rm -rf $(NAME)

re : fclean all