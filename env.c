/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:31 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/19 20:03:01 by aaltinto         ###   ########.fr       */
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
