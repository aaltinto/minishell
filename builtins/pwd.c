/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:25 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/19 18:46:40 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

void	*new_pwd(t_vars *vars)
{
	char	*pwd;
	int		i;
	char	**splitted;

	i = find_in_env(vars->env, "PWD");
	if (i == -1)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return (perror("getcwd"), NULL);
		return (ft_putendl_fd(pwd, 1), null_free(&pwd), NULL);
	}
	splitted = ft_split(vars->env[i], '=');
	ft_putendl_fd(splitted[1], 1);
	return (free_doubles(splitted), NULL);
}
