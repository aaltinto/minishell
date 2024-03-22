/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:13 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/19 20:01:33 by aaltinto         ###   ########.fr       */
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

int	set_env(t_vars *vars, char *to_find, char *to_set)
{
	int		i;
	char	*tmp;
	char	**new_env;

	i = find_in_env(vars->env, to_find);
	tmp = ft_strjoin(to_find, to_set);
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
	return (null_free(&tmp), 1);
}

char	*get_env(t_vars *vars, char *to_find)
{
	int		i;
	char	**splitted;
	char	*tmp;

	i = find_in_env(vars->env, to_find);
	if (i == -1)
		return (NULL);
	splitted = ft_split(vars->env[i], '=');
	if (!splitted)
		return (NULL);
	tmp = ft_strdup(splitted[1]);
	free_doubles(splitted);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int	new_cd(t_vars *vars)
{
	char	*old_path;
	char	*new_path;

	old_path = get_env(vars, "PWD=");
	if (!old_path)
		old_path = getcwd(NULL, 0);
	if (ft_strncmp(vars->input_parsed[1], "-", 2) == 0)
		return (get_oldpwd(vars), null_free(&old_path), 1);
	if (chdir(vars->input_parsed[1]) != 0)
		return (perror("minishell: cd"), null_free(&old_path), 0);
	new_path = getcwd(NULL, 0);
	set_env(vars, "PWD=", new_path);
	set_env(vars, "OLDPWD=", old_path);
	return (1);
}
