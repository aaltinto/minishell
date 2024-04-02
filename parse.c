/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:58 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/24 18:12:05 by aaltinto         ###   ########.fr       */
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
		return (tmp2);
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
		return (tmp2);
	}
	else
		return (ft_substr(input, i - j, j));
}

int	check_origin(t_vars *vars)
{
	int	check;

	check = 0;
	if (!vars->input)
		return (-1);
	if (ft_strncmp(vars->input, "", 1) == 0)
		return (-1);
	strip(vars->input);
	if (ft_strncmp(vars->input, "echo ", 5) == 0
		|| ft_strncmp(vars->input, "export ", 6) == 0)
		check = 1;
	vars->input_parsed
		= (char **)ft_calloc(ft_strlen(vars->input) + 1, sizeof(char *));
	if (!vars->input_parsed)
		return (err_msg("Allocation Error"), -1);
	return (check);
}

int	loop_parsing(t_vars *vars, int *p, int j, int check)
{
	int		i;
	char	quote_type;

	i = *p;
	if (vars->input[i] == '\"' || vars->input[i] == '\'')
	{
		quote_type = vars->input[i];
		while ((vars->input[++i] != '\0' && quote_type != vars->input[i]))
			++j;
		vars->input_parsed[vars->argc++] = cut_parse_quote(i, j, check, vars->input);
		if (!vars->input_parsed[vars->argc -1])
			return (free_doubles(vars->input_parsed), err_msg("Error"), 0);
	}
	else if (!is_space(vars->input[i]))
	{
		while (vars->input[i] != '\0' && (!is_space(vars->input[i])
				&& !is_quote(vars->input[i]) && ++i))
			++j;
		vars->input_parsed[vars->argc++] = cut_parse(i, j, check, vars->input);
		if (!vars->input_parsed[vars->argc -1] && i--)
			return (free_doubles(vars->input_parsed), err_msg("Error"), 0);
		if (is_quote(vars->input[i]))
			i --;
	}
	if (vars->input[i] == '\0')
		return (-1);
	*p = i;
	return (1);
}

int	parse(t_vars *vars, int count)
{
	int		i;
	int		ret;
	int		check;

	(void)count;
	check = check_origin(vars);
	if (check == -1)
		return (0);
	i = -1;
	vars->argc = 0;
	while (vars->input[++i] != '\0')
	{
		ret= loop_parsing(vars, &i, 0, check);
		if (!ret)
			return (0);
		if (ret == -1)
			break ;
		if (vars->input[0] == '\0')
			return (0);
	}
	vars->input_parsed[vars->argc++] = NULL;
	vars->argc = double_counter(vars->input_parsed);
	return (null_free(&vars->input), 1);
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
		else if (vars->input[i] == '*')
			wildcard(vars, i, 1);
		else if (vars->input[i] == ' ' && vars->input[i + 1] == '*')
			wildcard(vars, ++i, 0);
		else if (vars->input[i] == '$' && vars->input[i + 1] == '?')
		{
			if (!exit_status(vars, i))
				return (0);
		}
		else if ((vars->input[i] == '$' && vars->input[i + 1] == 40)
			|| (vars->input[i] == '$'
				&& !is_space(vars->input[i + 1]) && vars->input[i + 1] != '\0'
				&& env_find_dollar(vars, i -1, 0) == -1))
			return (null_free(&vars->input), 0);
		if (!vars->input || vars->input[i] == '\0')
			break ;
	}
	return (1);
}
