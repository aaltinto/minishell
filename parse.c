/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:58 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/22 16:47:05 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdio.h>	

int	parse(t_vars *vars, int count)
{
	int		i;
	int		j;
	int		check;
	char	quote_type;
	char	*input;
	char	*tmp;

	if (!vars->input)
		return (0);
	check = 0;
	input = ft_strdup(strip(vars->input));
	if (ft_strncmp(input, "echo ", 5) == 0
		|| ft_strncmp(input, "export ", 6) == 0)
		check = 1;
	vars->input_parsed
		= (char **)ft_calloc(ft_strlen(input) + 1, sizeof(char *));
	i = -1;
	quote_type = 0;
	if (ft_strncmp(input, "", 1) == 0)
		return (0);
	while (input[++i] != '\0')
	{
		if (input[i] == 39 || input[i] == 34)
		{
			quote_type = input[i];
			j = 0;
			i++;
			while ((input[i] != '\0' && quote_type != input[i]) && ++i)
				++j;
			if (is_space(input[i + 1]) && check)
			{
				tmp = ft_substr(input, i - j, j);
				vars->input_parsed[count++] = ft_strjoin(tmp, " ");
				null_free(&tmp);
			}
			else
				vars->input_parsed[count++] = ft_substr(input, i - j, j);
			quote_type = 0;
		}
		else if (!is_space(input[i]))
		{
			j = 0;
			while (input[i] != '\0' && (!is_space(input[i])
					&& !is_quote(input[i]) && ++i))
				++j;
			if (is_space(input[i]) && check)
			{
				tmp = ft_substr(input, i - j, j);
				vars->input_parsed[count++] = ft_strjoin(tmp, " ");
				null_free(&tmp);
			}
			else
				vars->input_parsed[count++] = ft_substr(input, i - j, j);
			i = i - 1;
		}
		if (input[i] == '\0')
			break ;
	}
	vars->input_parsed[count++] = NULL;
	vars->argc = double_counter(vars->input_parsed);
	return (null_free(&input), null_free(&vars->input), 1);
}

int	dolar_parse(t_vars *vars)
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
		if (quote_pass(vars, i, &quote_type, &in_quotes)
			|| (in_quotes && quote_type == '\''))
			continue ;
		else if (vars->input[i] == '$' && vars->input[i + 1] == '?')
			exit_status(vars, i);
		else if ((vars->input[i] == '$' && vars->input[i + 1] == 40
				&& exec_dollar_cmd(vars, i -1) == -1) || (vars->input[i] == '$'
				&& !is_space(vars->input[i + 1]) && vars->input[i + 1] != '\0'
				&& env_find_dollar(vars, i -1, 0) == -1))
			return (null_free(&vars->input), 0);
		if (vars->input[i] == '\0')
			break ;
	}
	return (1);
}
