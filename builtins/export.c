/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:21 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/19 20:00:58 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

char	**dup_env(t_vars *vars, char **to_dup)
{
	int		i;
	char	**new_env;

	i = double_counter(to_dup);
	vars->argc = double_counter(vars->input_parsed);
	new_env = malloc((i + vars->argc + 1) * sizeof(char *));
	i = -1;
	while (to_dup[++i])
	{
		new_env[i] = ft_strdup(to_dup[i]);
		if (!new_env[i])
			return (err_msg("Error"), NULL);
	}
	new_env[i] = NULL;
	return (new_env);
}

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
			printf("declare -x %s=\"%s\"\n", exports[0], (ft_strchr(vars->env[i], '=') + 1));
		vars->env[i] = tmp;
		free_doubles(exports);
	}
}

void	check_restore(t_vars *vars)
{
	int		i;
	int		index;
	int		del;
	int		count;
	char	**splited;
	char	**tmp;

	i = 0;
	del = 0;
	count = double_counter(vars->env);
	tmp = vars->input_parsed;
	while (vars->input_parsed[++i])
	{
		splited = ft_split(strip(vars->input_parsed[i]), '=');
		index = find_in_env(vars->env, splited[0]);
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

int	check_validity(t_vars *vars)
{
	int		i;
	int		j;
	int		check;
	char	*err;
	char	*tmp;
	char	**tmp2;
	char	*input;

	i = 0;
	while (vars->input_parsed[++i])
	{
		j = -1;
		tmp = ft_strdup(vars->input_parsed[i]);
		input = ft_strdup(strip(tmp));
		null_free(&tmp);
		printf("%s\n", input);
		while (input[++j])
		{
			if (input[j] != '=' && !ft_isalnum(input[j]))
			{
				if (vars->input_parsed[i - 1])
					tmp = ft_strchr(vars->input_parsed[i - 1], '=');
				printf("-%s-\n", tmp);
				if (tmp && tmp++)
					if (ft_strncmp(tmp, "= ", ft_strlen(tmp)) == 0)
						break ;
				tmp2 = ft_split(input, '=');
				write(1, "a\n", 2);
				if (tmp2)
				{
					check = 0;
					for (int i= 0; tmp2[0][i]; i++)
						if (!ft_isalnum(tmp2[0][i]))
							check = 1;
					printf("%d\n", check);
					free_doubles(tmp2);
					if (check == 0)
						break;
				}
				err = ft_strjoin("export: invalid identifier found: ",
						vars->input_parsed[i]);
				err_msg(err);
				null_free(&err);
				null_free(&vars->input_parsed[i]);
				break ;
			}
		}
		null_free(&input);
	}
	return (1);
}

int	new_export(t_vars *vars)
{
	int		i;
	int		i2;
	char	*check;
	char	*tmp;
	char	**new_env;

	if (!vars->input_parsed[1])
		return (print_vars(vars), 1);
	if (!check_validity(vars))
		return (2);
	check_restore(vars);
	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (2);
	i2 = double_counter(new_env) - 1;
	i = 0;
	tmp = NULL;
	check = NULL;
	while (vars->argc + 1 >= ++i)
	{
		if (!vars->input_parsed[i])
			continue ;
		printf("-%s-\n", vars->input_parsed[i]);
		if (ft_strchr(vars->input_parsed[i], '=') != 0)
			check = ft_strchr(vars->input_parsed[i], '=');
		if (check == NULL || ft_strncmp(check, "= ", 2) == 0
			|| (!is_space(*check++) && *check != '\0'))
		{
			new_env[++i2] = ft_strdup(strip(vars->input_parsed[i]));
			if (!new_env[i2])
				return (err_msg("Error"), 2);
		}
		else
		{
			if (vars->input_parsed[i + 1] != NULL)
				tmp = ft_strjoin(vars->input_parsed[i], strip(vars->input_parsed[i + 1]));
			else
				new_env[++i2] = ft_strdup(vars->input_parsed[i]);
			if (tmp != NULL)
				new_env[++i2] = ft_strdup(tmp);
			i++;
			null_free(&tmp);
		}
		check = NULL;
	}
	new_env[++i2] = NULL;
	free_doubles(vars->env);
	return (env_init(vars, new_env), free_doubles(new_env), 1);
}

void	re_init_env(t_vars *vars, int count, int del)
{
	char	**new_env;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (del == 0)
		return ;
	new_env = ft_calloc(sizeof(char *), (count - del + 1));
	while (count >= ++i)
	{
		if (!vars->env[i])
			continue ;
		new_env[j] = ft_strdup(vars->env[i]);
		j++;
	}
	free_doubles(vars->env);
	env_init(vars, new_env);
}

void	unset(t_vars *vars)
{
	int		i;
	int		j;
	int		count;
	int		del;
	char	**exports;

	if (vars->input_parsed[1] == NULL)
		return ;
	count = double_counter(vars->env);
	del = 0;
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
