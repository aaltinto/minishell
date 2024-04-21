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

int	is_logic(int chr, int chr2)
{
	if (chr == '\0')
		return (0);
	if (chr2 == '\0')
		return (0);
	return ((chr == '&' && chr2 == '&')
		|| (chr == '|' && chr2 == '|'));
}

void	delete_para(char ***var)
{
	char	**commands;
	int		i;
	int		j;

	commands = *var;
	i = -1;
	while (commands[++i])
	{
		j = -1;
		while (commands[i][++j])
			if (commands[i][j] == 40
				|| commands[i][j] == 41)
				commands[i][j] = 32;
	}
	*var = commands;
}

int	check_parantheses(t_vars *vars, int *p, int i, int *in_para)
{
	int	j;

	j = *p;
	while (vars->input[i + j] != '\0' && !is_logic(vars->input[i + j],
			vars->input[i + j + 1]))
	{
		if (vars->input[i + j] == 40)
			*in_para = 1;
		if (vars->input[i + j] == 41)
		{
			if (!(*in_para))
				return (err_msg("parentheses error"),
					null_free(&vars->input), 0);
			*in_para = 0;
		}
		j++;
	}
	*p = j;
	return (1);
}
