#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	is_builtin(t_vars *vars)
{
	if (ft_strncmp(vars->input_parsed[0], "cd", 20) == 0)
		return (new_cd(vars));
	if (ft_strncmp(vars->input_parsed[0], "pwd", 30) == 0)
		return (new_pwd(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "echo", 40) == 0)
		return (echo(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "export", 5) == 0)
		return (new_export(vars));
	if (ft_strncmp(vars->input_parsed[0], "env", 3) == 0)
		return (new_env(vars), 1);
	return (0);
}

int	something_familiar(t_vars *vars)
{
	if (!vars->input_parsed)
		return (1);
	else if (ft_strncmp("exit", vars->input_parsed[0], 4) == 0)
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", vars->input_parsed[0], 2) == 0)
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
	parse(vars);
	ret = something_familiar(vars);
	if (ret)
		return (ret);
	path_finder(vars);
	return (1);
}
