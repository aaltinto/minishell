#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdio.h>

int	parse(t_vars *vars, int count)
{
	int		i;
	int		len;
	int		in_quotes;
	int		j;

	if (!vars->input)
		return (0);
	len = ft_strlen(vars->input);
	vars->input_parsed = (char **)ft_calloc(len, sizeof(char *));
	i = -1;
	in_quotes = 0;
	j = 0;
	while (++i <= len)
	{
		if (vars->input[i] == '"' || vars->input[i] == '\'')
			in_quotes = !in_quotes;
		else if ((!in_quotes && is_space(vars->input[i])) || !vars->input[i])
		{
			if (!in_quotes && i > j)
			{
				if (vars->input[j] == '"' || vars->input[j] == '\'')
					j++;
				if (vars->input[i - 1] == '"' || vars->input[i - 1] == '\'')
					i--;
				vars->input_parsed[count++] = ft_substr(vars->input, j, i - j);
			}
			j = i + 1;
		}
	}
	vars->input_parsed[count++] = NULL;
	null_free(&vars->input);
	return (1);
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
	tmp = split_string(vars->input, var);
	j = find_in_env(vars->env, var + 1);
	null_free(&var);
	null_free(&tmp[1]);
	if (j != -1)
		tmp[1] = ft_strdup(ft_strchr(vars->env[j], '=') + 1);
	vars->env = original;
	append_doubles(&vars->input, tmp, 1);
	if (!vars->input)
		return (free_doubles(tmp), -1);
	return (free_doubles(tmp), 1);
}

static int	replace_input(t_vars *vars, char *var, char **tmp)
{
	char	**cmd;
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
		return (err_msg("Error", 1), free_doubles(tmp), -1);
	cmd = ft_split(var, ' ');
	if (!cmd)
		return (err_msg("Error", 1), -1);
	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	marche(&new_vars, vars->env, 0);
	new_vars.input = ft_strdup(var);
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
// static int	replace_input(t_vars *vars, char *var, char **tmp)
// {
// 	char	**cmd;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	while (tmp[1][++i])
// 		if (tmp[1][i] != 41)
// 			j++;
// 	var = ft_substr(tmp[1], 2, j - 1);
// 	if (!var)
// 		return (err_msg("Error", 1), free_doubles(tmp), -1);
// 	cmd = ft_split(var, ' ');
// 	if (!cmd)
// 		return (err_msg("Error", 1), -1);
// 	path_finder(vars, cmd[0], cmd, 0);
// 	null_free(&tmp[1]);
// 	tmp[1] = ft_strdup(vars->output);
// 	append_doubles(&vars->input, tmp, 1);
// 	if (ft_strncmp(vars->input, "", 2) == 0)
// 		return (-1);
// 	return (1);
// }

int	exec_dollar_command(t_vars *vars, int i)
{
	int		j;
	int		open_paren;
	int		last_paren;
	int		len;
	int		check;
	char	*var;
	char	**tmp;

	len = ft_strlen(vars->input);
	last_paren = 0;
	j = 0;
	check = 0;
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
			return (err_msg("Parentheses error", 1), -1);
	}
	if (open_paren != 0)
		return (err_msg("Parentheses error", 1), -1);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Error", 1), -1);
	tmp = split_string(vars->input, var);
	null_free(&var);
	if (!tmp)
		return (err_msg("Error", 1), -1);
	return (replace_input(vars, var, tmp));
}

int	exit_status(t_vars *vars, int i)
{
	char	*deli;
	char	**tmp;

	deli = ft_substr(vars->input, i, 2);
	tmp = split_string(vars->input, deli);
	null_free(&tmp[1]);
	tmp[1] = ft_itoa(vars->exit_stat);
	append_doubles(&vars->input, tmp, 1);
	return (1);
}

int	dolar_parse(t_vars *vars)
{
	int		i;
	int		quote_type;

	i = -1;
	quote_type = 0;
	while (vars->input[++i])
	{
		if (vars->input[i] == '\"')
			quote_type = !quote_type;
		else if (quote_type == 0 && vars->input[i] == '\'')
			while (vars->input[++i] != '\'')
				;
		else if (vars->input[i] == '$' && vars->input[i + 1] == '?')
			exit_status(vars, i);
		else if (vars->input[i] == '$' && vars->input[i + 1] == 40
			&& exec_dollar_command(vars, i -1) == -1)
			return (null_free(&vars->input), 0);
		else if (vars->input[i] == '$' && !is_space(vars->input[i + 1])
			&& vars->input[i + 1] != '\0' && env_find_dollar(vars, i -1) == -1)
			return (null_free(&vars->input), 0);
		else if (vars->input[i] == '<' && vars->input[i + 1] == '<'
			&& ++i && ++i && heredoc(vars, i) == 0)
			return (null_free(&vars->input), 0);
		else if (vars->input[i] == '<' && open_file(vars, i -1) == -1)
			return (null_free(&vars->input), 0);
		else if (vars->input[i] == '>' && vars->input[i + 1] == '>'
			&& ++i && ++i && append_output(vars, i - 2) == -1)
			return (null_free(&vars->input), 0);
		else if (vars->input[i] == '>' && output_file(vars, i) == -1)
			return (null_free(&vars->input), 0);
	}
	return (1);
}
