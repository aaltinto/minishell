
#include "minishell.h"
#include "gnl/get_next_line.h"

int	execute(char *path, t_vars *vars, int *pipe_fd)
{
	char	*output;
	char	*tmp;
	char	**splitted;
	int		i;

	close(pipe_fd[1]);
	output = get_next_line(pipe_fd[0]);
	splitted = ft_split(output, '\n');
	free(output);
	if (!splitted)
		return (perror("Split"), 0);
	i = -1;
	while (splitted[++i])
	{
		tmp = ft_strdup(splitted[i]);
		free(splitted[i]);
		splitted[i] = ft_strjoin(tmp, " ");
		free(tmp);
	}
	append_doubles(&vars->output, splitted);
	free_doubles(splitted);
	return (1);
}

int	pipe_exec(char *path, t_vars *vars, char **argv, int condition)
{
	int		p_id;
	int		pipe_fd[2];

	vars->output = NULL;
	if (pipe(pipe_fd) == -1)
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
			perror("execute problem");
	}
	else if (!condition)
		execute(path, vars, pipe_fd);
	return (waitpid(p_id, NULL, 0), close(pipe_fd[0]), 1);
}
