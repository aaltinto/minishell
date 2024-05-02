/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:09:49 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/16 11:09:54 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

int	is_logic(int chr, int chr2)
{
	if (chr == '\0')
		return (0);
	if (chr2 == '\0')
		return (0);
	return ((chr == '&' && chr2 == '&')
		|| (chr == '|' && chr2 == '|'));
}

int	split_coms(t_vars *vars, int *p, int i)
{
	int	j;

	j = *p;
	while (vars->input[i + j] != '\0' && !is_logic(vars->input[i + j],
			vars->input[i + j + 1]))
		j++;
	*p = j;
	return (1);
}

int	check_commands(char **commands, t_vars **child)
{
	if (!ft_strncmp(commands[0], "", 2) || (!ft_strncmp(commands[double_counter \
	(commands) - 1], "&", 2) || !ft_strncmp(commands[double_counter(commands) - \
	1], "|", 2)))
		return (err_msg(SYNTAX_ERR), 1);
	*child = malloc(sizeof(t_vars) * (double_counter(commands) + 1));
	if (!(*child))
		return (err_msg("Malloc error"), 1);
	return (0);
}

int	check_para(t_vars *vars)
{
	int	i;
	int	inpara;

	i = -1;
	inpara = 0;
	while (vars->input[++i])
	{
		if (vars->input[i] == 40)
		{
			inpara ++;
			vars->input[i] = 32;
		}
		if (vars->input[i] == 41)
		{
			if (!inpara)
				return (err_msg("paranthesis error"),
					null_free(&vars->input), 0);
			inpara --;
			vars->input[i] = 32;
		}
	}
	if (inpara != 0)
		return (err_msg("paranthesis error"), null_free(&vars->input), 0);
	return (1);
}
