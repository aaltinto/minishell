#include "minishell.h"
#include "libft/libft.h"
#include "gnl/get_next_line.h"
#include <stdio.h>
#include <unistd.h>

int	get_list(t_vars *vars, int *pipe_fd)
{
	char	*output;
	char	*tmp;
	char	**splitted;
	int		i;

	close(pipe_fd[1]);
	vars->output = get_next_line(pipe_fd[0]);
	if (!vars->output)
		return (0);
	return (1);
}


int	exec_ls(t_vars *vars)
{
	int		p_id;
	int		pipe_fd[2];
	char	**argv;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	p_id = fork();
	if (p_id == -1)
		return (perror("fork"), 0);
	if (p_id == 0)
	{
		argv = ft_split("ls", ' ');
		if (!argv)
			return (killer(vars), exit(EXIT_FAILURE), 0);
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (killer(vars), perror("dup2"), 0);
		if (execve("/bin/ls", argv, vars->env) == -1)
			return (killer(vars), perror("execve"), close(pipe_fd[1]),
				close(pipe_fd[0]), exit(EXIT_FAILURE), 0);
	}
	get_list(vars, pipe_fd);
	close(pipe_fd[0]);
	return (waitpid(p_id, NULL, 0), 1);
}

int	transform_output(t_vars *vars)
{
	char	**splitted;
	char	*tmp;
	int		i;

	splitted = ft_split(vars->output, '\n');
	null_free(&vars->output);
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

int	find_in_list(t_vars *vars, char *var, int rev)
{
	char	**split;
	char	**list;
	char	*tmp;
	int		len;
	int		i;
	int		j;

	if (ft_strncmp(var, "*", 2) == 0)
		return (transform_output(vars));
	split = ft_split(vars->output, '\n');
	null_free(&vars->output);
	if (!split)
		return (err_msg("Malloc error"), 0);
	list = ft_calloc(double_counter(split), sizeof(char *));
	if (!list)
		return (err_msg("Malloc error"), free_doubles(split), 0);
	i = -1;
	j = -1;
	if (!rev)
		*var++;
	if (rev)
	{
		len = ft_strlen(var);
		var[len -1] = '\0';
		len--;
	}
	while (split[++i])
	{
		if (!rev)
		{
			tmp = ft_strrchr(split[i], var[0]);
			len = ft_strlen(tmp);
		}
		else
			tmp = split[i];
		if (!tmp)
			continue ;
		if (ft_strncmp(var, tmp, len) == 0)
		{
			if (!rev && len != ft_strlen(var))
				continue ;
			tmp = ft_strdup(split[i]);
			list[++j] = ft_strjoin(tmp, " ");
			null_free(&tmp);
			if (!list[j])
				return (err_msg("strdup error"), free_doubles(split),
					free_doubles(list), 0);
		}
		tmp = NULL;
	}
	free_doubles(split);
	if (j == -1)
		return (free_doubles(list), 2);
	if (!append_doubles(&vars->output, list, 0))
		return (free_doubles(list), 0);
	return (free_doubles(list), 1);
}

int	wildcard(t_vars *vars, int i, int rev)
{
	int		j;
	char	*var;
	char	*var_tmp;
	char	**tmp;
	char	*tmp2;

	j = 0;
	if (rev)
	{
		while(i >= 0 && !is_space(vars->input[i]))
		{
			i--;
			j++;
		}
		var = ft_substr(vars->input, i + 1, j + 1);
	}
	else
	{
		while(vars->input[i] && !is_space(vars->input[i]))
		{
			i++;
			j++;
		}
		var = ft_substr(vars->input, i - j, j);
	}
	exec_ls(vars);
	var_tmp = ft_strdup(var);
	find_in_list(vars, var, rev);
	null_free(&var);
	if (!vars->output)
		return (1);
	tmp = split_string(vars->input, var_tmp);
	null_free(&var_tmp);
	null_free(&tmp[1]);
	tmp[1] = ft_strdup(vars->output);
	append_doubles(&vars->input, tmp, 1);
	return (free_doubles(tmp), 1);
}
