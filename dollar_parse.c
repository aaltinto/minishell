/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:02:43 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/22 16:46:04 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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

int	exec_dollar_cmd(t_vars *vars, int i)
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

int	env_find_dollar(t_vars *vars, int i, int j)
{
	char	*var;
	char	**tmp;
	char	**original;

	original = vars->env;
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
		tmp[1] = ft_strdup(ft_strchr(vars->env[j], '=') + 1);
	if (!tmp[1])
		return (free_doubles(tmp), -1);
	vars->env = original;
	if (!append_doubles(&vars->input, tmp, 1) || !vars->input)
		return (free_doubles2((void **)tmp, 3), -1);
	return (free_doubles2((void **)tmp, 3), 1);
}
