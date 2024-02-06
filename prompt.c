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
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", input, 3) == 0)
		return (printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars, input))
		return (1);
	else
		return (0);

}

char	*wait_close(int	quote_check, int type, char *input)
{
	char	*new_input;
	char	*new_input;
	int		i;

	while (1)
	{
		new_input = readline(">");
		i = -1;
		while (new_input[++i])
		{
			if (type && new_input[i] == '\"')
				quote_check++;
			if (!type && new_input[i] == '\'')
				quote_check++;
		}
		if (quote_check % 2 == 0)
			break ;

	}
}

int	single_quote(char *input)
{
	int	i;
	int	quote_check;

	i = -1;
	quote_check = 0;
	while (input[++i] != '\0')
		if (input[i] == '\'')
			quote_check++;
	if (quote_check % 2 != 0)
		wait_close(quote_check, 0, input);
	return (-1);
}

int	double_quote(char *input)
{
	int		i;
	int		quote_check;
	char	*tmp;

	i = -1;
	quote_check = 0;
	while (input[++i] != '\0')
		if (input[i] == '\"')
			quote_check++;
	if (quote_check % 2 != 0)
		wait_close(quote_check, 1, input);
	return (-1);
}

int	quote(char *input, t_vars *vars)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '\"')
			i = double_quote(input);
		else if (input[i] == '\'')
			i = single_quote(input);
		if (i == -1)
			break ;
	}
}

int	handle_prompt(char *input, t_vars *vars)
{
	int		ret;

	quote(input, vars);
	ret = something_familiar(input, vars);
	if (ret)
		return (ret);
	path_finder(input, vars);
	return (1);
}
