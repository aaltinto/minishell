/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:21 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/22 17:26:09 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

void	print_vars(t_vars *vars)
{
	char	**exports;
	char	*tmp;
	int		i;

	i = -1;
	while (vars->env[++i])
	{
		tmp = vars->env[i];
		exports = ft_split(vars->env[i], '=');
		if (!exports)
			return ;
		if (ft_strchr(vars->env[i], '=') && !exports[1])
			printf("declare -x %s=\"\"\n", exports[0]);
		else if (!exports[1])
			printf("declare -x %s\n", exports[0]);
		else
			printf("declare -x %s=\"%s\"\n", exports[0],
				(ft_strchr(vars->env[i], '=') + 1));
		vars->env[i] = tmp;
		free_doubles(exports);
	}
}

void	check_restore(t_vars *vars, int count)
{
	int		i;
	int		index;
	int		del;
	char	**splited;
	char	**tmp;

	i = 0;
	del = 0;
	count = double_counter(vars->env);
	tmp = vars->input_parsed;
	while (vars->input_parsed[++i])
	{
		splited = ft_split(vars->input_parsed[i], '=');
		if (!splited || !splited[0])
			continue ;
		index = find_in_env(vars->env, strip(splited[0]));
		free_doubles(splited);
		if (index == -1)
			continue ;
		vars->env[index] = NULL;
		del++;
	}
	if (del)
		re_init_env(vars, count, del);
	vars->input_parsed = tmp;
}

int	err_clean(t_vars *vars, int i)
{
	char	*err;

	err = ft_strjoin("export: invalid identifier found: ",
			vars->input_parsed[i]);
	if (!err)
		return (err_msg("strjoin error"), 0);
	err_msg(err);
	null_free(&err);
	null_free(&vars->input_parsed[i]);
	return (1);
}

int	illegal_char_check(char *input, int position)
{
	char	**tmp;
	int		check;
	int		i;

	tmp = ft_split(input, '=');
	if (!tmp || !tmp[0])
		return (free_doubles(tmp), 0);
	if (double_counter(tmp) == 1 && input[0] == '=')
		return (free_doubles(tmp), 1);
	if (tmp && position)
	{
		check = 0;
		i = -1;
		while (tmp[0][++i])
			if (!ft_isalnum(tmp[0][i]))
				check = 1;
		free_doubles(tmp);
		if (check == 0)
			return (0);
	}
	if (position)
		return (1);
	return (0);
}

int	check_validity(t_vars *vars)
{
	int		i;
	int		j;
	char	*tmp;
	char	*input;

	i = 0;
	while (vars->input_parsed[++i])
	{
		j = -1;
		input = ft_strdup(strip(vars->input_parsed[i]));
		if (illegal_char_check(input, 0))
			err_clean(vars, i);
		while (!vars->input_parsed[i] && input[++j])
		{
			if (input[j] != '=' && !ft_isalnum(input[j]))
				continue ;
			if (vars->input_parsed[i - 1])
				tmp = ft_strchr(vars->input_parsed[i - 1], '=');
			if (tmp && tmp++ && ft_strncmp(tmp, "= ", 3) == 0)
				break ;
			if (illegal_char_check(input, 1))
				err_clean(vars, i);
		}
		null_free(&input);
	}
	return (1);
}

char	*add_to_env(t_vars *vars, int *i)
{
	char	*check;
	char	*tmp;

	check = NULL;
	if (ft_strchr(vars->input_parsed[*i], '=') != 0)
		check = ft_strchr(vars->input_parsed[*i], '=');
	if (check == NULL || ft_strncmp(check, "= ", 2) == 0
		|| (!is_space(*check++) && *check != '\0'))
		return (ft_strdup(strip(vars->input_parsed[*i])));
	else
	{
		if (vars->input_parsed[*i + 1] != NULL)
		{
			tmp = ft_strjoin(vars->input_parsed[*i],
					strip(vars->input_parsed[*i + 1]));
			if (tmp != NULL)
				return ((*i)++, tmp);
		}
		else
			return (ft_strdup(vars->input_parsed[*i]));
	}
	return (NULL);
}

int	new_export(t_vars *vars)
{
	int		i;
	int		i2;
	char	**new_env;

	if (!vars->input_parsed[1])
		return (print_vars(vars), 1);
	if (!check_validity(vars))
		return (2);
	check_restore(vars, 0);
	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (2);
	i2 = double_counter(new_env) - 1;
	i = 0;
	while (vars->argc + 1 >= ++i)
	{
		if (!vars->input_parsed[i])
			continue ;
		new_env[++i2] = add_to_env(vars, &i);
		if (!new_env[i2])
			return (2);
	}
	new_env[++i2] = NULL;
	free_doubles(vars->env);
	return (env_init(vars, new_env), free_doubles(new_env), 1);
}

void	unset(t_vars *vars, int del, int count)
{
	int		i;
	int		j;
	char	**exports;

	if (vars->input_parsed[1] == NULL)
		return ;
	i = -1;
	while (vars->env[++i])
	{
		j = 0;
		while (vars->input_parsed[++j])
		{
			exports = ft_split(vars->env[i], '=');
			if (ft_strncmp(exports[0], vars->input_parsed[j],
					ft_strlen(exports[0])) == 0)
			{
				free(vars->env[i]);
				vars->env[i] = NULL;
				free_doubles(exports);
				del++;
				break ;
			}
		}
	}
	re_init_env(vars, count, del);
}
