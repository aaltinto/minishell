/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:42 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/22 16:35:52 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

static int	fd_open_operations(t_vars *vars, char *var)
{
	int		fd;
	char	*file;

	if (vars->file_opened)
		if (dup2(vars->origin_stdin, STDIN_FILENO) == -1)
			return (perror("dup2"), -1);
	file = ft_substr(strip(var + 1), 0, ft_strlen(var));
	if (!file)
		return (err_msg("Error"), -1);
	fd = open(file, O_RDONLY);
	null_free(&file);
	if (fd < 0)
		return (err_msg("No such file or directory"), -1);
	vars->origin_stdin = dup(STDIN_FILENO);
	if (vars->origin_stdin == -1)
		return (perror("Error"), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), -1);
	vars->file_opened = 1;
	if (!vars->input)
		return (close(fd), -1);
	return (close(fd), 1);
}

int	open_file(t_vars *vars, int i)
{
	int		j;
	int		ret;
	int		check;
	char	*var;
	char	**tmp;

	j = 0;
	check = 1;
	while (vars->input[++i] && (check || !is_space(vars->input[i])))
		if (j++ && !is_space(vars->input[i]))
			check = 0;
	if (check == 1)
		return (
			err_msg("minishell: syntax error near unexpected token"), -1);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Error"), -1);
	tmp = split_string(strip(vars->input), var);
	if (!tmp)
		return (free(var), -1);
	null_free(&tmp[1]);
	if (!append_doubles(&vars->input, tmp, 1))
		return (err_msg("Error"), null_free(&var), free_doubles(tmp), -1);
	free_doubles2((void **)tmp, 3);
	ret = fd_open_operations(vars, var);
	return (null_free(&var), ret);
}

static int	fd_output_operations(t_vars *vars, char *var)
{
	int		fd;
	char	*file;
	char	*tmp;

	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), -1);
	tmp = strip(var + 1);
	file = ft_substr(tmp, 0, ft_strlen(var));
	null_free(&tmp);
	free(var);
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	null_free(&file);
	if (fd < 0)
		return (perror("minishell"), -1);
	vars->origin_stdout = dup(STDOUT_FILENO);
	if (vars->origin_stdout == -1)
		return (perror("minishell: dup"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), -1);
	vars->file_created = 1;
	if (!vars->input)
		return (close(fd), -1);
	close(fd);
	return (1);
}

int	output_file(t_vars *vars, int i)
{
	int		j;
	int		check;
	char	*var;
	char	**tmp;

	j = 0;
	check = 1;
	while (vars->input[++i] && (check || !is_space(vars->input[i])))
		if (j++ && !is_space(vars->input[i]))
			check = 0;
	if (j++ <= 1)
		return (
			err_msg("minishell: syntax error near unexpected token"), -1);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Error"), -1);
	tmp = split_string(strip(vars->input), var);
	if (!tmp)
		return (null_free(&var), err_msg("Error"), -1);
	null_free(&tmp[1]);
	if (!append_doubles(&vars->input, tmp, 1))
		return (free_doubles(tmp), -1);
	free_doubles(tmp);
	return (fd_output_operations(vars, var));
}
