/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:34 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/15 16:12:35 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gnl/get_next_line.h"
#include "libft/libft.h"
#include <stdio.h>

int	execute(t_vars *vars, int *pipe_fd)
{
	char	*output;
	char	*tmp;
	char	**splitted;
	int		i;

	output = get_next_line(pipe_fd[0]);
	if (!output)
		return (close(pipe_fd[0]), 0);
	close(pipe_fd[0]);
	splitted = ft_split(output, '\n');
	null_free(&output);
	if (!splitted)
		return (perror("split"), 0);
	i = -1;
	while (splitted[++i])
	{
		if (splitted[i + 1] == NULL)
			break ;
		tmp = ft_strdup(splitted[i]);
		null_free(&splitted[i]);
		splitted[i] = ft_strjoin(tmp, " ");
		null_free(&tmp);
	}
	return (append_doubles(&vars->output, splitted, 0),
		free_doubles(splitted), close(pipe_fd[0]), 1);
}

int	wexitstatus(int status)
{
	return ((status >> 8) & 0x000000ff);
}

int	pipe_exec(char *path, t_vars *vars, char **argv, int condition)
{
	int		p_id;
	int		status;
	int		pipe_fd[2];

	if (!condition && pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	p_id = fork();
	if (p_id == -1)
		return (perror("fork"), 0);
	if (p_id == 0)
	{
		if (!condition && close(pipe_fd[0]) && dup2(pipe_fd[1],
				STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
		if (execve(path, argv, vars->env) == -1)
		{
			if (!condition)
			{
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}
			killer(vars);
			return (perror("execve"), exit(EXIT_FAILURE), 0);
		}
	}
	else if (!condition)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		execute(vars, pipe_fd);
	}
	waitpid(p_id, &status, 0);
	vars->exit_stat = wexitstatus(status);
	return (1);
}

int	path_finder(t_vars *vars, char *cmd, char **argv, int condition)
{
	char	**split_path;
	char	*this_is_the_way;
	char	*err;
	char	*tmp;
	char	*tmp2;
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
	tmp = ft_strjoin("/", cmd);
	i = -1;
	while (split_path && split_path[++i])
	{
		this_is_the_way = ft_strjoin(split_path[i], tmp);
		if (access(this_is_the_way, F_OK) == 0)
			return (free_doubles(split_path), null_free(&tmp),
				pipe_exec(this_is_the_way, vars, argv, condition),
				null_free(&this_is_the_way), 1);
		null_free(&this_is_the_way);
	}
	tmp2 = getcwd(NULL, 0);
	this_is_the_way = ft_strjoin(tmp2, tmp);
	if (access(this_is_the_way, F_OK) == 0)
		return (pipe_exec(this_is_the_way, vars, argv, condition),
			null_free(&this_is_the_way), null_free(&tmp), null_free(&tmp2), 1);
	null_free(&this_is_the_way);
	null_free(&tmp2);
	err = ft_strjoin("minishell: command not found: ", cmd);
	null_free(&tmp);
	free_doubles(split_path);
	return (err_msg(err), null_free(&err), 0);
}
