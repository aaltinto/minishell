/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:31 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/15 16:12:32 by aaltinto         ###   ########.fr       */
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
