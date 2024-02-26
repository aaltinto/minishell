#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	is_builtin(t_vars *vars)
{
	if (ft_strncmp(vars->input_parsed[0], "cd", 2) == 0)
		return (new_cd(vars));
	if (ft_strncmp(vars->input_parsed[0], "pwd", 3) == 0)
		return (new_pwd(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "echo", 4) == 0)
		return (echo(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "export", 5) == 0)
		return (new_export(vars));
	if (ft_strncmp(vars->input_parsed[0], "env", 3) == 0)
		return (new_env(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "unset", 5) == 0)
		return (unset(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "denem", 5) == 0)
		return (unset(vars), 1);
	return (0);
}

int	something_familiar(t_vars *vars)
{
	if (ft_strncmp("exit", vars->input_parsed[0], 4) == 0)
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

	if (ft_strncmp("", vars->input, 2) == 0)
		return (free(vars->input), 1);
	vars->input_parsed = NULL;
	quote(vars);
	add_history(vars->input);
	dolar_parse(vars);
	parse(vars, 0);
	ret = something_familiar(vars);
	if (ret)
		return (ret);
	path_finder(vars, vars->input_parsed[0], vars->input_parsed, 1);
	if (vars->output)
		free(vars->output);
	return (free_doubles(vars->input_parsed), 1);
}
