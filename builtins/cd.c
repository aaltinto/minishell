/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:13 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/16 17:09:39 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void	get_oldpwd(t_vars *vars)
{
	int		i;
	char	**tmp;

	i = find_in_env(vars->env, "OLDPWD=");
	if (i == -1)
	{
		err_msg("minishell: cd: OLDPWD not set");
		return ;
	}
	tmp = ft_split(vars->env[i], '=');
	ft_putendl_fd(tmp[1], 1);
	free_doubles(tmp);
}

int	new_cd(t_vars *vars)
{
	int		i;
	char	*old_path;
	char	*new_path;
	char	*tmp;
	char	**new_env;

	old_path = getcwd(NULL, 0);
	if (ft_strncmp(vars->input_parsed[1], "-", 2) == 0)
		return (get_oldpwd(vars), null_free(&old_path), 1);
	if (chdir(vars->input_parsed[1]) != 0)
		return (perror("minishell: cd"), 0);
	new_path = getcwd(NULL, 0);
	i = find_in_env(vars->env, "PWD=");
	tmp = ft_strjoin("PWD=", new_path);
	free(new_path);
	if (i != -1)
	{
		free(vars->env[i]);
		vars->env[i] = ft_strdup(tmp);
	}
	free(tmp);
	i = find_in_env(vars->env, "OLDPWD");
	tmp = ft_strjoin("OLDPWD=", old_path);
	free(old_path);
	if (i != -1)
	{
		free(vars->env[i]);
		vars->env[i] = ft_strdup(tmp);
	}
	else
	{
		new_env = dup_env(vars, vars->env);
		i = double_counter(vars->env);
		new_env[i] = ft_strdup(tmp);
		new_env[++i] = NULL;
		free_doubles(vars->env);
		env_init(vars, new_env);
		free_doubles2((void **)new_env, i + vars->argc - 1);
	}
	return (free(tmp), 1);
}
