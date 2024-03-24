/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:34 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/24 18:00:50 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gnl/get_next_line.h"
#include "libft/libft.h"
#include <stdio.h>

int	wexitstatus(int status)
{
	return ((status >> 8) & 0x000000ff);
}

int	pipe_exec(char *path, t_vars *vars, char **argv)
{
	int		p_id;
	int		status;

	p_id = fork();
	if (p_id == -1)
		return (perror("fork"), 0);
	if (p_id == 0)
	{
		if (execve(path, argv, vars->env) == -1)
		{
			killer(vars);
			return (perror("execve"), exit(EXIT_FAILURE), 0);
		}
	}
	waitpid(p_id, &status, 0);
	vars->exit_stat = wexitstatus(status);
	return (1);
}

int	execute(t_vars *vars, char *cmd, char **argv, char	**split_path)
{
	char	*this_is_the_way;
	char	*tmp;
	char	*tmp2;
	int		i;

	i = -1;
	tmp = ft_strjoin("/", cmd);
	while (split_path && split_path[++i])
	{
		this_is_the_way = ft_strjoin(split_path[i], tmp);
		if (access(this_is_the_way, F_OK) == 0)
			return (null_free(&tmp), pipe_exec(this_is_the_way, vars, argv),
				null_free(&this_is_the_way), 1);
		null_free(&this_is_the_way);
	}
	tmp2 = getcwd(NULL, 0);
	this_is_the_way = ft_strjoin(tmp2, tmp);
	if (access(this_is_the_way, F_OK) == 0)
		return (pipe_exec(this_is_the_way, vars, argv),
			null_free(&this_is_the_way), null_free(&tmp), null_free(&tmp2), 1);
	null_free(&this_is_the_way);
	null_free(&tmp);
	null_free(&tmp2);
	return (0);
}

int	path_finder(t_vars *vars, char *cmd, char **argv)
{
	char	**split_path;
	char	*err;
	int		i;

	if (argv == NULL)
		return (0);
	i = find_in_env(vars->env, "PATH=");
	if (i != -1)
	{
		split_path = ft_split(vars->env[i], ':');
		if (!split_path)
			return (err_msg("Split error!"), 0);
	}
	else
		split_path = NULL;
	if (execute(vars, cmd, argv, split_path))
		return (free_doubles(split_path), 1);
	free_doubles(split_path);
	err = ft_strjoin("minishell: command not found: ", cmd);
	return (err_msg(err), null_free(&err), 0);
}
