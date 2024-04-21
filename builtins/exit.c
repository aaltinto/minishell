/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:37:30 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/16 16:14:16 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

int	check_digits(t_vars *vars, char **split)
{
	int		i;
	int		j;

	i = 0;
	while (split[++i])
	{
		j = -1;
		while (split[i][++j])
		{
			if (j == 0 && (split[i][j] == '+' || split[i][j] == '-'))
				continue ;
			if (!ft_isdigit(split[i][j]))
				return (vars->exit_stat = 255, 
					err_msg("numeric argument required"), 0);
		}
	}
	return (1);
}

char	**input_reparse(t_vars *vars)
{
	char	*tmp;
	char	**split;
	char	*input;

	tmp = NULL;
	if (!append_doubles(&tmp, vars->input_parsed, 0))
		return (NULL);
	input = destroy_quotes(tmp);
	null_free(&tmp);
	if (!input)
		return (NULL);
	split = ft_split(input, ' ');
	null_free(&input);
	if (!split)
		return (err_msg("Split error"), NULL);
	return (split);
}

int	exit_setter(t_vars *vars)
{
	int		num;
	char	**split;

	if (!vars->input_parsed)
		return (ft_putendl_fd("\nexit", 1), 1);
	if (vars->argc <= 1)
		return (ft_putendl_fd("exit", 1), 1);
	split = input_reparse(vars);
	if (!split)
		return (vars->exit_stat = 1, 0);
	num = double_counter(split);
	if (num <= 1)
		return (vars->exit_stat = 0, 1);
	else if (num >= 3)
		return (err_msg("Too many arguments"), vars->exit_stat = 1, 0);
	if (!check_digits(vars, split))
		return (free_doubles(split), 0);
	num = ft_atoi(split[1]);
	if (num > 255)
		num = num % 256;
	vars->exit_stat = num;
	return (num);
}
