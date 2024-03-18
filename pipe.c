/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:04 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/17 16:27:38 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

int	child_process(int **pipes, int pipe_count, t_vars *vars, int i)
{
	t_vars	new_vars;
	char	**argv;
	int		j;

	argv = ft_split(vars->input, '|');
	if (i != 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i != pipe_count)
		dup2(pipes[i][1], STDOUT_FILENO);
	j = -1;
	while (++j < pipe_count)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
	marche(&new_vars, vars->env, 0);
	new_vars.input = ft_strdup(argv[i]);
	handle_prompt(&new_vars, 0);
	free_doubles(argv);
	return (reset_fds(vars), exit(EXIT_SUCCESS), 1);
}

int	pipe_piping(int **pipes, int pipe_count, t_vars *vars)
{
	int		i;
	pid_t	*pid;

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

int	pipe_counter(t_vars *vars)
{
	int		i;
	int		pipe_count;
	int		in_quotes;
	char	quote_type;

	i = -1;
	pipe_count = 0;
	while (vars->input[++i])
	{
		if (vars->input[i] == '\"' || vars->input[i] == '\'')
		{
			if (!quote_type)
				quote_type = vars->input[i];
			if (quote_type == vars->input[i])
			{
				in_quotes = !in_quotes;
				if (!in_quotes)
					quote_type = '\0';
				continue ;
			}
		}
		if (!in_quotes && vars->input[i] == '|')
			pipe_count++;
	}
	return (pipe_count);
}

int	pipe_parse(t_vars *vars)
{
	int		i;
	int		**pipes;
	int		pipe_count;

	if (!vars->input)
		return (0);
	pipe_count = pipe_counter(vars);
	if (pipe_count == 0)
		return (0);
	pipes = malloc(sizeof(int *) * pipe_count);
	i = -1;
	while (++i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (1);
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), 1);
	}
	return (pipe_piping(pipes, pipe_count, vars),
		free_doubles2((void **)pipes, pipe_count), 1);
}
