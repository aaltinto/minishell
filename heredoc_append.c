#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include "libft/libft.h"
#include <readline/readline.h>

static int	fd_append_operations(t_vars *vars, char *var)
{
	int		fd;
	char	*file;

	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	file = ft_substr(strip(var + 2), 0, ft_strlen(var));
	free(var);
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (err_msg("No such file or directory", 1), 0);
	vars->origin_stdout = dup(STDOUT_FILENO);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), 0);
	vars->file_created = 1;
	if (!vars->input)
		return (close(fd), -1);
	close(fd);
	return (1);
}

int	append_output(t_vars *vars, int i)
{
	int		j;
	int		check;
	char	*var;
	char	**tmp;

	j = 3;
	check = 1;
	printf("append\n");
	i += 2;
	while (vars->input[++i] && (check || !is_space(vars->input[i])))
		if (j++ && !is_space(vars->input[i]))
			check = 0;
	if (j <= 3 || check)
		return (
			err_msg("minishell: syntax error near unexpected token `newline'", \
			1), -1);
	var = ft_substr(vars->input, i - j, j);
	printf("var = %s\nj = %d\n", var, j);
	if (!var)
		return (err_msg("Error", 1), -1);
	tmp = split_string(strip(vars->input), var);
	null_free(&tmp[1]);
	append_doubles(&vars->input, tmp, 1);
	return (fd_append_operations(vars, var));
}

int	handle_eof(t_vars *vars, char *delimeter)
{
	int		fd[2];
	char	*new_input;

	if (pipe(fd) == -1)
		return (perror("pipe"), null_free(&vars->input), -1);
	while (1)
	{
		new_input = readline("> ");
		if (!new_input || ft_strncmp(new_input, delimeter,
				ft_strlen(new_input)) == 0)
			break ;
		ft_putendl_fd(new_input, fd[1]);
	}
	if (!new_input)
		ft_putchar_fd('\n', 1);
	vars->origin_stdin = dup(STDIN_FILENO);
	vars->file_opened = 1;
	if (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1
		|| close(fd[1]) == -1)
		return (perror("dup2/close"), null_free(&vars->input), -1);
	return (1);
}

int	heredoc(t_vars *vars, int i)
{
	int		j;
	int		check;
	char	*var;
	char	*delimeter;
	char	**tmps;

	j = 3;
	check = 1;
	while (vars->input[++i] && (check || !is_space(vars->input[i])))
		if (j++ && !is_space(vars->input[i]))
			check = 0;
	if (j <= 3 || check)
		return (
			err_msg("minishell: syntax error near unexpected token `newline'", \
			1), 0);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Error", 1), 0);
	tmps = split_string(strip(vars->input), var);
	null_free(&tmps[1]);
	append_doubles(&vars->input, tmps, 1);
	delimeter = ft_substr(strip(var + 2), 0, ft_strlen(var));
	handle_eof(vars, delimeter);
	return (vars->input != NULL);
}
