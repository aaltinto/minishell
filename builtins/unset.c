/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:07 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/23 17:51:04 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

int	unset(t_vars *vars, int del, int count)
{
	int		i;
	int		j;
	char	**exports;

	if (vars->input_parsed[1] == NULL)
		return (1);
	i = -1;
	while (vars->input_parsed[++i])
	{
		if (check_val(vars, NULL, vars->input_parsed[i], 1))
			continue ;
		j = 0;
		while (vars->env[++j])
		{
			exports = ft_split(vars->env[j], '=');
			if (!exports)
				return (err_msg("allocation error"), 0);
			if (ft_strncmp(exports[0], vars->input_parsed[i],
					ft_strlen(exports[0])) == 0)
				if (null_free(&vars->env[j]), del++)
					break ;
			free_doubles(exports);
		}
	}
	return (re_init_env(vars, count, del));
}
