/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:21 by aaltinto          #+#    #+#             */
/*   Updated: 2024/05/05 14:35:34 by aaltinto         ###   ########.fr       */
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

char	*find_before_eq(char *var)
{
	char	**split;
	char	*tmp;

	split = ft_split(var, '=');
	if (!split)
		return (err_msg("Split error"), NULL);
	tmp = strip(split[0]);
	if (!tmp)
		return (err_msg("Strip error"), free_doubles(split), NULL);
	return (free_doubles(split), tmp);
}

int	check_val(t_vars *vars, char ***new_env, char *export)
{
	char	**tmp2;
	char	*tmp;
	int		i;
	int		count;

	count = double_counter(*new_env);
	tmp = find_before_eq(export);
	if (!tmp)
		return (0);
	if (ft_strncmp(tmp, "", 1) == 0 || !tmp)
		return (null_free(&tmp), err_msg("not a valid identifier!"), 1);
	if (illegal_char_check(tmp))
		return (null_free(&tmp), 1);
	i = find_in_env(*new_env, tmp, double_counter(*new_env));
	if (null_free(&tmp) && i == -1)
		return (null_free(&tmp), 0);;
	null_free(&(*new_env)[i]);
	tmp2 = re_init_double(*new_env, count, 1);
	if (!tmp2)
		return (err_msg("env couldn't re-init"), 1);
	free_doubles2((void **)(*new_env), count);
	*new_env = dup_env(vars, tmp2);
	if (!new_env)
		return (err_msg("env couldn't re-init"), 1);
	return (free_doubles(tmp2), 0);
}


int	new_export(t_vars *vars, int ret, int i)
{
	int		i2;
	char	**new_env;
	char	*tmp;

	if (!check_restore(vars, 0))
		return (0);
	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (0);
	i2 = double_counter(new_env) - 1;
	while ((vars->argc + 1) > ++i)
	{
		if (!vars->input_parsed[i])
			continue ;
		new_env[i2 + 1] = NULL;
		tmp = find_before_eq(vars->input_parsed[i]);
		if (find_in_env(new_env, tmp, i2) != -1)
			i2--;
		null_free(&tmp);
		if (check_val(vars, &new_env, vars->input_parsed[i]))
		{
			ret = 0;
			continue ;
		}
		new_env[++i2] = ft_strdup(vars->input_parsed[i]);
		if (!new_env[i2])
			return (free_doubles(new_env), err_msg("Something wrong"), 2);
	}
	return (new_env[++i2] = NULL, free_doubles(vars->env), env_init(vars, \
	new_env), free_doubles2((void **)new_env, i2), ret);
}
