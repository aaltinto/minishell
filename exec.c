
#include "minishell.h"
#include "gnl/get_next_line.h"
#include "libft/libft.h"
#include <stdio.h>

int	execute(char *path, t_vars *vars, int *pipe_fd)
{
	char	*output;
	char	*tmp;
	char	**splitted;
	int		i;

	close(pipe_fd[1]);
	output = get_next_line(pipe_fd[0]);
	if (!output)
		return (0);
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
		free_doubles(splitted), 1);
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
		close(pipe_fd[0]);
		if (!condition && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
		if (execve(path, argv, vars->env) == -1)
			return (perror("execve"), close(pipe_fd[1]), close(pipe_fd[0]),
				exit(EXIT_FAILURE), 0);
	}
	else if (!condition)
		execute(path, vars, pipe_fd);
	if (!condition)
		close(pipe_fd[0]);
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
	int		i;

	if (argv == NULL)
		return (0);
	i = find_in_env(vars->env, "PATH=");
	split_path = ft_split(vars->env[i], ':');
	if (!split_path)
		return (err_msg("Error!", 1), 0);
	tmp = ft_strjoin("/", cmd);
	i = -1;
	while (split_path[++i])
	{
		this_is_the_way = ft_strjoin(split_path[i], tmp);
		if (access(this_is_the_way, F_OK) == 0)
			return (free_doubles(split_path), null_free(&tmp),
				pipe_exec(this_is_the_way, vars, argv, condition), 1);
		null_free(&this_is_the_way);
	}
	this_is_the_way = getcwd(NULL, 0);
	this_is_the_way = ft_strjoin(this_is_the_way, tmp);
	if (access(this_is_the_way, F_OK) == 0)
		return (pipe_exec(this_is_the_way, vars, argv, condition),
			null_free(&this_is_the_way), null_free(&tmp), 1);
	err = ft_strjoin("minishell: command not found: ", cmd);
	null_free(&tmp);
	return (err_msg(err, 1), null_free(&err), 0);
}