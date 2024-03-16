/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:04 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/15 17:22:48 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

int	child_process(int **pipes, int pipe_count, t_vars *vars, int i)
{
	char	**cmd;
	char	**argv;
	int		j;

	argv = ft_split(vars->input, '|');
	if (i != 0)
	{
		dup2(pipes[i - 1][0], STDIN_FILENO);
		close(pipes[i - 1][0]);
	}
	if (i != pipe_count)
	{
		close(pipes[i][0]);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	j = -1;
	while (++j < pipe_count)
	{
		if (j != i - 1)
			close(pipes[j][0]);
		if (j != i)
			close(pipes[j][1]);
	}
	cmd = ft_split(argv[i], ' ');
	return (path_finder(vars, cmd[0], cmd, 1), exit(EXIT_SUCCESS), 1);
}

int	pipe_piping(int **pipes, int pipe_count, t_vars *vars)
{
	char	**cmd;
	pid_t	*pid;
	int		j;
	int		i;

	pid = malloc(sizeof(pid_t) * (pipe_count + 1));
	i = -1;
	while (++i < pipe_count + 1)
	{
		pid[i] = fork();
		if (pid[i] == -1)
			return (perror("fork"), 0);
		if (pid[i] == 0)
			child_process(pipes, pipe_count, vars, i);
	}
	i = -1;
	while (++i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	i = -1;
	g_l = 0;
	while (++i < pipe_count + 1)
		waitpid(pid[i], &g_l, 0);
	vars->exit_stat = wexitstatus(g_l);
	return (free(pid), null_free(&vars->input), 1);
}

int	pipe_parse(t_vars *vars)
{
	int		i;
	int		pipe_count;
	int		**pipes;

	i = -1;
	pipe_count = 0;
	if (!vars->input)
		return (1);
	while (vars->input[++i])
		if (vars->input[i] == '|')
			pipe_count++;
	if (pipe_count == 0)
		return (0);
	pipes = malloc(sizeof(int *) * pipe_count);
	i = -1;
	while (++i < pipe_count)
		pipes[i] = malloc(sizeof(int) * 2);
	i = -1;
	while (++i < pipe_count)
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), 1);
	return (pipe_piping(pipes, pipe_count, vars), free_doubles2((void **)pipes, pipe_count), 1);
}
