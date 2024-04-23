/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:21 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/13 16:01:57 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <unistd.h>

int	illegal_char_check(char *str)
{
	int		i;
	char	*err;

	i = -1;
	while (str[++i])
	{
		if ((i == 0 && (ft_isalpha(str[i]) || str[i] == '_')) || \
	(i != 0 && (str[i] == '=' || str[i] == '_' || ft_isalnum(str[i]))))
			continue ;
		err = ft_strjoin("export: invalid identifier found: ", str);
		if (!err)
			return (err_msg("strjoin error"), 1);
		err_msg(err);
		null_free(&err);
		return (1);
	}
	return (0);
}

int	check_validity(t_vars *vars, char *export)
{
	char	**split;
	char	*tmp;

	(void)vars;
	split = ft_split(export, '=');
	if (!split)
		return (err_msg("Split error"), 0);
	if (ft_strncmp(split[0], "", 1) == 0 || !split[0])
		return (free_doubles(split), err_msg("not a valid identifier!"), 1);
	tmp = strip(split[0]);
	if (illegal_char_check(tmp))
		return (free_doubles(split), null_free(&tmp), 1);
	return (free_doubles(split), null_free(&tmp), 0);
}

char	*append_args(t_vars *vars, int	*index)
{
	char	**append;
	char	*ret;
	char	*tmp;
	int		i;
	int		j;

	i = *index;
	tmp = NULL;
	ret = NULL;
	append = ft_calloc(sizeof(char *), vars->argc + 1);
	if (append == 0)
		return (err_msg("malloc error"), NULL);
	j = -1;
	while (vars->input_parsed[i] && tmp == NULL)
	{
		tmp = ft_strchr(vars->input_parsed[i], ' ');
		append[++j] = ft_strdup(vars->input_parsed[i]);
		if (!append[j])
			return (free_doubles(append), err_msg("malloc error"), NULL);
		i++;
	}
	append[++j] = NULL;
	append_doubles(&ret, append, 0);
	*index = i;
	return (free_doubles(append), ret);
}

char	*add_to_env(t_vars *vars, int *index)
{
	char	*ret;
	char	*tmp;
	int		i;

	i = *index;
	ret = NULL;
	tmp = ft_strchr(vars->input_parsed[i], ' ');
	if ((tmp && tmp[0] == ' ') || !vars->input_parsed[i + 1])
	{
		ret = ft_strdup(vars->input_parsed[i]);
		i++;
		if (!ret)
			return (err_msg("malloc error"), NULL);
	}
	else
		ret = append_args(vars, &i);
	*index = i;
	return (ret);
}

int	new_export(t_vars *vars, int ret)
{
	int		i;
	int		i2;
	char	**new_env;

	if (!vars->input_parsed[1])
		return (print_vars(vars), 1);
	check_restore(vars, 0);
	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (2);
	i2 = double_counter(new_env) - 1;
	i = 0;
	while ((vars->argc + 2) > ++i)
	{
		if (!vars->input_parsed[i])
			continue ;
		new_env[++i2] = add_to_env(vars, &i);
		if (!new_env[i2])
			return (free_doubles(new_env), err_msg("Something wrong"), 2);
		if (check_validity(vars, new_env[i2]) && null_free(&new_env[i2]))
			ret = 0;
	}
	return (new_env[++i2] = NULL, free_doubles(vars->env), env_init(vars, \
	new_env), free_doubles(new_env), ret);
}
