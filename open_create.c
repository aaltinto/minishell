/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:17:04 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/22 16:15:57 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	reset_fds(t_vars *vars)
{
	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	if (vars->file_opened)
	{
		if (dup2(vars->origin_stdin, STDIN_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	return (1);
}

int	open_fds_parse(t_vars *vars)
{
	int		i;
	char	quote_type;
	int		in_quotes;

	if (!vars->input)
		return (0);
	i = -1;
	quote_type = 0;
	in_quotes = 0;
	while (vars->input[++i])
	{
		if (quote_pass(vars, i, &quote_type, &in_quotes) || in_quotes)
			continue ;
		else if ((vars->input[i] == '<' && vars->input[i + 1] == '<' && \
		vars->input[i + 2] != '<' && ++i && heredoc(vars, ++i) == 0)
			|| (vars->input[i] == '<' && open_file(vars, i -1) == -1))
			return (null_free(&vars->input), 0);
		else if ((vars->input[i] == '>' && vars->input[i + 1] == '>'
				&& append_output(vars, i) == -1) || (vars->input[i] == '>'
				&& output_file(vars, i) == -1))
			return (null_free(&vars->input), 0);
		if (vars->input[i] == '\0')
			break ;
	}
	return (1);
}