/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:21 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/23 17:51:04 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

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

int	illegal_char_check(t_vars *vars, int i, char **try)
{
	int		j;
	char	*tmp;

	j = -1;
	while (vars->input_parsed[i] && try[0][++j])
	{
		if ((j == 0 && (ft_isalpha(try[0][j]) || try[0][j] == '_')) || \
	(j != 0 && (try[0][j] == '=' || try[0][j] == '_' || ft_isalnum(try[0][j]))))
			continue ;
		if (vars->input_parsed[i - 1])
			tmp = ft_strchr(vars->input_parsed[i - 1], '=');
		if (tmp && ft_strncmp(tmp, "= ", 3) != 0)
			break ;
		if (!err_clean(vars, i))
			return (0);
	}
	return (1);
}

int	check_validity(t_vars *vars)
{
	int		i;
	char	**try;

	i = 0;
	while (vars->input_parsed[++i])
	{
		try = ft_split(vars->input_parsed[i], '=');
		if (!try)
			return (err_msg("Split error"), 0);
		if (ft_strncmp(ft_strchr(vars->input_parsed[i], '='),
				vars->input_parsed[i], ft_strlen(vars->input_parsed[i])) == 0)
			if (err_clean(vars, i))
				return (0);
		if (illegal_char_check(vars, i, try))
			return (0);
		free_doubles(try);
	}
	return (1);
}

char	*add_to_env(t_vars *vars, int *index)
{
	char	*check;
	char	*ret;
	int		i;

	i = *index;
	check = NULL;
	if (ft_strchr(vars->input_parsed[i], '=') != 0)
		check = ft_strchr(vars->input_parsed[i], '=');
	if (check == NULL || ft_strncmp(check, "= ", 2) == 0
		|| (!is_space(*check++) && *check != '\0'))
		ret = ft_strdup(strip(vars->input_parsed[i]));
	else if (vars->input_parsed[i + 1] != NULL)
	{
		ret = ft_strjoin(vars->input_parsed[i],
				strip(vars->input_parsed[i + 1]));
		if (ret != NULL)
			*index = i + 1;
	}
	else
		ret = ft_strdup(vars->input_parsed[i]);
	return (ret);
}

int	new_export(t_vars *vars)
{
	int		i;
	int		i2;
	char	**new_env;

	if (!vars->input_parsed[1])
		return (print_vars(vars), 1);
	if (check_validity(vars))
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
