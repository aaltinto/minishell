/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:02:43 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/24 17:34:20 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>

int	exit_status(t_vars *vars, int i)
{
	char	*deli;
	char	**tmp;

	deli = ft_substr(vars->input, i, 2);
	if (!deli)
		return (err_msg("Malloc error"), 0);
	tmp = split_string(vars->input, deli);
	null_free(&deli);
	if (!tmp)
		return (err_msg("Malloc error"), 0);
	null_free(&tmp[1]);
	tmp[1] = ft_itoa(vars->exit_stat);
	if (!tmp[1])
		return (err_msg("Error"), 0);
	if (!append_doubles(&vars->input, tmp, 1))
		return (free_doubles2((void **)tmp, 3), err_msg("Error"), 0);
	free_doubles2((void **)tmp, 3);
	return (1);
}

int	env_find_dollar(t_vars *vars, int i, int j)
{
	char	*var;
	char	**tmp;
	char	**original;

	original = vars->env;
	while (vars->input[++i] && vars->input[i] != ' ' && vars->input[i] != '\''
		&& vars->input[i] != '\"')
		j++;
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (-1);
	tmp = split_string(vars->input, var);
	if (!tmp)
		return (null_free(&var), -1);
	j = find_in_env(vars->env, var + 1);
	null_free(&var);
	null_free(&tmp[1]);
	if (j != -1)
		tmp[1] = ft_strdup(ft_strchr(vars->env[j], '=') + 1);
	if (!tmp[1])
		return (free_doubles(tmp), -1);
	vars->env = original;
	if (!append_doubles(&vars->input, tmp, 1) || !vars->input)
		return (free_doubles2((void **)tmp, 3), -1);
	return (free_doubles2((void **)tmp, 3), 1);
}
