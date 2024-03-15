#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <readline/history.h>
#include <stdio.h>

int	is_builtin(t_vars *vars)
{
	if (ft_strncmp(vars->input_parsed[0], "cd", 2) == 0)
		return (new_cd(vars), 1);
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
	return (0);
}

int	something_familiar(t_vars *vars)
{
	if (vars->input_parsed == NULL)
		return (1);
	if (ft_strncmp("exit", vars->input_parsed[0], 5) == 0)
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", vars->input_parsed[0], 3) == 0)
		return (printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars))
		return (1);
	else
		return (0);
}

int	reset_fds(t_vars *vars)
{
	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	if (vars->file_opened)
	{
		if (dup2(vars->origin_stdin, STDIN_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	return (1);
}

int	handle_prompt(t_vars *vars, int condition)
{
	int		ret;

	if (!vars->input)
		return (2);
	if (ft_strncmp("", vars->input, 2) == 0)
	{
		vars->exit_stat = 127;
		return (1);
	}
	quote(vars);
	if (vars->hist != -1 && condition)
		add_history(vars->input);
	dolar_parse(vars);
	if (!condition && !vars->input)
		return (killer(vars), 1);
	if (pipe_parse(vars))
		return (reset_fds(vars), 1);
	if (!vars->input)
		return (reset_fds(vars));
	if (!parse(vars, 0))
		return (0);
	ret = something_familiar(vars);
	if (!ret)
		path_finder(vars, strip(vars->input_parsed[0]), vars->input_parsed, condition);
	if (!reset_fds(vars))
		return (0);
	if (!condition)
		killer(vars);
	return (ret);
}
