/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:05 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/24 17:54:16 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <readline/history.h>
#include <stdio.h>

int	is_builtin(t_vars *vars)
{
	char	*input;
	int		ret;

	input = strip(vars->input_parsed[0]);
	if (ft_strncmp(input, "cd", 2) == 0)
		ret = new_cd(vars);
	else if (ft_strncmp(input, "pwd", 4) == 0)
		ret = new_pwd(vars);
	else if (ft_strncmp(input, "echo", 5) == 0)
		ret = (echo(vars), 1);
	else if (ft_strncmp(input, "export", 7) == 0)
		ret = (new_export(vars));
	else if (ft_strncmp(input, "env", 4) == 0)
		ret = (new_env(vars), 1);
	else if (ft_strncmp(input, "unset", 6) == 0)
		ret = (unset(vars, 0, double_counter(vars->env)));
	else
		ret = -1;
	return (null_free(&input), ret);
}

int	something_familiar(t_vars *vars)
{
	if (vars->input_parsed == NULL)
		return (1);
	if (ft_strncmp("exit", vars->input_parsed[0], 5) == 0)
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", vars->input_parsed[0], 3) == 0)
		return (printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars) != -1)
		return (1);
	else
		return (0);
}

int	handle_prompt(t_vars *vars, int condition)
{
	int		ret;

	if (!vars->input)
		return (2);
	if (ft_strncmp("", vars->input, 2) == 0)
		return (vars->exit_stat = 127, 1);
	if (quote(vars), vars->hist != -1 && condition)
		add_history(vars->input);
	if (!dolar_parse(vars) || pipe_parse(vars))
		return (reset_fds(vars), 1);
	if (!condition && !vars->input)
		return (killer(vars), 1);
	if (!vars->input)
		return (reset_fds(vars));
	open_fds_parse(vars);
	if (!parse(vars, 0))
		return (reset_fds(vars), 0);
	ret = something_familiar(vars);
	if (!ret)
		path_finder(vars, vars->input_parsed[0], vars->input_parsed);
	if (!reset_fds(vars))
		return (0);
	if (!condition)
		killer(vars);
	return (ret);
}
