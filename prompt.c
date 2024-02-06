#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>

int	is_builtin(t_vars *vars, char *input)
{
	char	**command;

	command = ft_split(input, ' ');
	if (ft_strncmp(command[0], "cd", 3) == 0)
		return (new_cd(vars, command[1]));
	if (ft_strncmp(command[0], "pwd", 4) == 0)
		return (new_pwd(vars), 1);
	if (ft_strncmp(command[0], "echo", 5) == 0)
		return (echo(vars, input), 1);
	return (0);
}

int	something_familiar(char *input, t_vars *vars)
{
	if (ft_strncmp("", input, 1) == 0)
		return (1);
	else if (ft_strncmp("exit", input, 5) == 0)
		return (printf("See ya 🫡"), 2);
	else if (ft_strncmp("hi", input, 3) == 0)
		return (printf("hi baby 😘"), 1);
	else if (is_builtin(vars, input))
		return (1);
	else
		return (0);

}

char	*wait_close(int	quote_check)
{
	char	*input;

	while (1)
	{
		input = readline(">");
		
	}
}

int	quote(char *input, t_vars *vars)
{
	int	quote_check;
	int	i;
	char	*

	quote_check = 0;
	i = -1;
	while (input[++i])
		if (input[i] == '\"' || input[i] == '\'')
			quote_check++;
	if (quote_check % 2 != 0)
		wait_close(quote_check);
}

int	handle_prompt(char *input, t_vars *vars)
{
	int		ret;


	ret = something_familiar(input, vars);
	if (ret)
		return (ret);
	path_finder(input, vars);
	return (1);
}
