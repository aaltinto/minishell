/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:58 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/17 15:55:44 by aaltinto         ###   ########.fr       */
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
	if (ft_strncmp(input, "echo ", 5) == 0 || ft_strncmp(input, "export ", 6) == 0)
		check = 1;
	vars->input_parsed = (char **)ft_calloc(ft_strlen(input) + 1, sizeof(char *));
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
			while (input[i] != '\0' && (!is_space(input[i]) && !is_quote(input[i]) && ++i))
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

int	env_find_dollar(t_vars *vars, int i)
{
	int		j;
	char	*var;
	char	**tmp;
	char	**original;

	original = vars->env;
	j = 0;
	while (vars->input[++i] && vars->input[i] != ' ' && vars->input[i] != '\''
		&& vars->input[i] != '\"')
		j++;
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (-1);
	tmp = split_string(vars->input, var);
	if (!tmp)
		return (null_free(&var), -1);
	j = find_in_env(vars->env, var + 1);
	null_free(&var);
	null_free(&tmp[1]);
	if (j != -1)
	{
		tmp[1] = ft_strdup(ft_strchr(vars->env[j], '=') + 1);
		if (!tmp[1])
			return (free_doubles(tmp), -1);
	}
	vars->env = original;
	append_doubles(&vars->input, tmp, 1);
	if (!vars->input)
		return (free_doubles2((void **)tmp, 3), -1);
	return (free_doubles2((void **)tmp, 3), 1);
}

static int	replace_input(t_vars *vars, char *var, char **tmp)
{
	int		fd[2];
	t_vars	new_vars;
	int		i;
	int		i2;
	int		j;

	i = ft_strlen(tmp[1]) + 1;
	while (--i > 0)
		if (tmp[1][i] != 41)
			break ;
	j = 0;
	i2 = 0;
	while (++i2 < i -1)
		++j;
	var = ft_substr(tmp[1], 2, j - 1);
	if (!var)
		return (err_msg("Error"), free_doubles(tmp), -1);
	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	marche(&new_vars, vars->env, 0);
	new_vars.input = ft_strdup(var);
	null_free(&var);
	handle_prompt(&new_vars, 0);
	null_free(&tmp[1]);
	if (new_vars.output == NULL)
		return (-1);
	tmp[1] = ft_strdup(new_vars.output);
	append_doubles(&vars->input, tmp, 1);
	if (ft_strncmp(vars->input, "", 2) == 0)
		return (-1);
	return (1);
}

int	exec_dollar_command(t_vars *vars, int i)
{
	int		j;
	int		open_paren;
	int		last_paren;
	char	*var;
	char	**tmp;

	last_paren = 0;
	j = 0;
	open_paren = 0;
	while (vars->input[++i] && ++last_paren)
	{
		if (vars->input[i] == 40)
			open_paren++;
		else if (vars->input[i] == 41)
		{
			j = last_paren;
			open_paren--;
		}
		if (open_paren < 0)
			return (err_msg("Parentheses error"), -1);
	}
	if (open_paren != 0)
		return (err_msg("Parentheses error"), -1);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Error"), -1);
	tmp = split_string(vars->input, var);
	null_free(&var);
	if (!tmp)
		return (err_msg("Error"), -1);
	i = replace_input(vars, var, tmp);
	return (free_doubles(tmp), i);
}

int	exit_status(t_vars *vars, int i)
{
	char	*deli;
	char	**tmp;

	deli = ft_substr(vars->input, i, 2);
	if (!deli)
		return (0);
	tmp = split_string(vars->input, deli);
	null_free(&deli);
	null_free(&tmp[1]);
	tmp[1] = ft_itoa(vars->exit_stat);
	append_doubles(&vars->input, tmp, 1);
	free_doubles2((void **)tmp, 3);
	return (1);
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
		if (in_quotes && quote_type == '\'')
			continue ;
		else if (vars->input[i] == '$' && vars->input[i + 1] == '?')
			exit_status(vars, i);
		else if (vars->input[i] == '$' && vars->input[i + 1] == 40
			&& exec_dollar_command(vars, i -1) == -1)
			return (null_free(&vars->input), 0);
		else if (vars->input[i] == '$' && !is_space(vars->input[i + 1])
			&& vars->input[i + 1] != '\0' && env_find_dollar(vars, i -1) == -1)
			return (null_free(&vars->input), 0);
		if (vars->input[i] == '\0')
			break ;
	}
	return (1);
}
