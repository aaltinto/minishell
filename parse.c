/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:58 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/16 15:50:27 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdio.h>

char	*cut_parse_quote(int i, int j, int check, char *input)
{
	char	*tmp;
	char	*tmp2;

	if (is_space(input[i + 1]) && check)
	{
		tmp = ft_substr(input, i - j, j);
		if (!tmp)
			return (NULL);
		tmp2 = ft_strjoin(tmp, " ");
		if (!tmp2)
			return (null_free(&tmp), NULL);
		return (null_free(&tmp), tmp2);
	}
	else
		return (ft_substr(input, i - j, j));
}

char	*cut_parse(int i, int j, int check, char *input)
{
	char	*tmp;
	char	*tmp2;

	if (is_space(input[i]) && check)
	{
		tmp = ft_substr(input, i - j, j);
		if (!tmp)
			return (NULL);
		tmp2 = ft_strjoin(tmp, " ");
		if (!tmp2)
			return (null_free(&tmp), NULL);
		return (null_free(&tmp), tmp2);
	}
	else
		return (ft_substr(input, i - j, j));
}

int	loop_parsing2(t_vars *vars, int j, int check)
{
	char	quote_type;

	if (vars->input[vars->i] == '\"' || vars->input[vars->i] == '\'')
	{
		quote_type = vars->input[vars->i];
		while ((vars->input[++(vars->i)] != '\0'
				&& quote_type != vars->input[vars->i]))
			++j;
		vars->input_parsed[++vars->argc] = cut_parse_quote(vars->i, j, check, \
			vars->input);
		if (!vars->input_parsed[vars->argc])
			return (free_doubles(vars->input_parsed),
				err_msg("Parse error"), 0);
		vars->i++;
	}
	return (1); 
}

int	loop_parsing(t_vars *vars, int j, int check)
{
	if (!loop_parsing2(vars, j, check))
		return (0);
	if (!is_space(vars->input[vars->i]))
	{
		while (vars->input[vars->i] != '\0' && (!is_space(vars->input[vars->i])
				&& !is_quote(vars->input[vars->i]) && ++(vars->i)))
			++j;
		vars->input_parsed[++vars->argc]
			= cut_parse(vars->i, j, check, vars->input);
		if (!vars->input_parsed[vars->argc] && (vars->i)--)
			return (free_doubles(vars->input_parsed),
				err_msg("Parse error"), 0);
		if (is_quote(vars->input[vars->i]))
			vars->i --;
	}
	if (vars->input[vars->i] == '\0')
		return (-1);
	return (1);
}

int	parse(t_vars *vars, int count)
{
	int		ret;
	int		check;

	(void)count;
	check = check_origin(vars);
	if (check == -1)
		return (0);
	vars->i = -1;
	vars->argc = -1;
	while (vars->input[++(vars->i)] != '\0')
	{
		ret = loop_parsing(vars, 0, check);
		if (!ret)
			return (0);
		if (ret == -1)
			break ;
		if (vars->input[0] == '\0')
			return (0);
	}
	if (ft_strncmp(vars->input_parsed[vars->argc], "", 1) != 0)
		vars->argc++;
	vars->input_parsed[++vars->argc] = NULL;
	vars->argc = double_counter(vars->input_parsed);
	return (null_free(&vars->input), 1);
}
