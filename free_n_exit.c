/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_n_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:37 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/16 13:59:57 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

int	killer(t_vars *vars)
{
	free_doubles(vars->env);
	free_doubles(vars->input_parsed);
	null_free(&vars->input);
	null_free(&vars->output);
	null_free(&vars->user_pwd);
	return (1);
}

void	err_msg(char *msg)
{
	ft_putendl_fd(msg, 2);
}

int	free_doubles(char **str)
{
	int	i;

	if (!str)
		return (1);
	i = -1;
	while (str[++i])
		null_free(&str[i]);
	free(str);
	str = NULL;
	return (1);
}

void	free_doubles2(void **str, int j)
{
	int		i;

	if (!str)
		return ;
	i = -1;
	while (j >= ++i)
	{
		if (str[i])
			null_free((char **)&str[i]);
	}
	free(str);
}

int	null_free(char **var)
{
	if (!*var)
		return (1);
	free(*var);
	*var = NULL;
	return (1);
}
