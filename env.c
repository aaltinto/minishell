/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:31 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/23 14:12:22 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	env_init(t_vars *vars, char **env)
{
	int	i;

	i = double_counter(env);
	vars->env = ft_calloc(sizeof(char *), (i + 1));
	if (!vars->env)
		exit(EXIT_FAILURE);
	i = -1;
	while (env[++i])
	{
		vars->env[i] = ft_strdup(env[i]);
		if (!vars->env[i])
			return (free_doubles(vars->env), 0);
	}
	vars->env[i] = NULL;
	return (1);
}

char	**dup_env(t_vars *vars, char **to_dup)
{
	int		i;
	char	**new_env;

	i = double_counter(to_dup);
	vars->argc = double_counter(vars->input_parsed);
	new_env = malloc((i + vars->argc + 1) * sizeof(char *));
	i = -1;
	while (to_dup[++i])
	{
		new_env[i] = ft_strdup(to_dup[i]);
		if (!new_env[i])
			return (err_msg("Error"), NULL);
	}
	new_env[i] = NULL;
	return (new_env);
}

void	re_init_env(t_vars *vars, int count, int del)
{
	char	**new_env;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (del == 0)
		return ;
	new_env = ft_calloc(sizeof(char *), (count - del + 1));
	while (count >= ++i)
	{
		if (!vars->env[i])
			continue ;
		new_env[j] = ft_strdup(vars->env[i]);
		j++;
	}
	free_doubles(vars->env);
	env_init(vars, new_env);
}

int	find_in_env(char **env, char *to_find)
{
	int		i;
	char	**tmp;

	i = -1;
	while (env[++i])
	{
		tmp = ft_split(env[i], '=');
		if (ft_strncmp(tmp[0], to_find, ft_strlen(tmp[0])) == 0)
			return (free_doubles(tmp), i);
		free_doubles(tmp);
	}
	return (-1);
}
