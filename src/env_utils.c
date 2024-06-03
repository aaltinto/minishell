/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:53:35 by aaltinto          #+#    #+#             */
/*   Updated: 2024/05/17 13:53:38 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>

char	**re_init_double(char **env, int count, int del)
{
	char	**new_env;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if (del == 0)
		return (env);
	new_env = malloc(sizeof(char *) * (count - del + 2));
	if (!new_env)
		return (err_msg("Malloc error"), NULL);
	while (count >= ++i)
	{
		if (!env[i])
			continue ;
		new_env[++j] = ft_strdup(env[i]);
		if (!new_env[j])
			return (free_doubles(new_env),
				err_msg("Strdup error"), NULL);
	}
	return (new_env[++j] = NULL, new_env);
}

int	update_env(t_vars *vars, t_vars *child)
{
	int i;

	free_doubles(vars->env);
	vars->env = (char **)malloc(sizeof(char *) * (double_counter(child->env) + 1));
	i = -1;
	while (child->env[++i])
	{
		vars->env[i] = ft_strdup(child->env[i]);
		if (!vars->env[i])
			return (err_msg("error\nenv corrupted!"), \
		free_doubles2((void **)vars->env, i), free_doubles(child->env), 0);
	}
	vars->env[++i] = NULL;
	free_doubles(child->env);
	return (1);
}