#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	is_builtin(t_vars *vars)
{
	char	**command;

	command = ft_split(vars->input, ' ');
	if (ft_strncmp(command[0], "cd", 3) == 0)
		return (free_doubles(command), new_cd(vars));
	if (ft_strncmp(command[0], "pwd", 4) == 0)
		return (free_doubles(command), new_pwd(vars), 1);
	if (ft_strncmp(command[0], "echo", 5) == 0)
		return (free_doubles(command), echo(vars), 1);
	return (0);
}

int	something_familiar(t_vars *vars)
{
	if (ft_strncmp("", vars->input, 1) == 0)
		return (1);
	else if (ft_strncmp("exit", vars->input, 5) == 0)
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", vars->input, 3) == 0)
		return (printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars))
		return (1);
	else
		return (0);
}


int	handle_prompt(t_vars *vars)
{
	int		ret;

	quote(vars);
	ret = something_familiar(vars);
	if (ret)
		return (ret);
	path_finder(vars);
	return (1);
}
