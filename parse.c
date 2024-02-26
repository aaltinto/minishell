#include "minishell.h"

int	parse(t_vars *vars, int count)
{
	int		i;
	int		len;
	int		in_quotes;
	int		j;

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
	free(vars->input);
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
	free(var);
	free(tmp[1]);
	if (j != -1)
		tmp[1] = ft_strdup(ft_strchr(vars->env[j], '=') + 1);
	else
		tmp[1] = NULL;
	vars->env = original;
	append_doubles(&vars->input, tmp);
	return (free_doubles(tmp), 1);
}

int	exec_dollar_command(t_vars *vars, int i)
{
	int		j;
	int		len;
	int		check;
	char	*var;
	char	**tmp;
	char	**cmd;

	len = ft_strlen(vars->input);
	j = 0;
	check = 0;
	while (vars->input[++i] && vars->input[i - 1] != 41)
	{
		if (vars->input[i] == 40 && check++)
			return (err_msg("Parentheses error", 1), 0);
		j++;
	}
	if (i == len && vars->input[i - 1] != 41)
		return (err_msg("Parentheses error", 1), 0);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Error", 1), 0);
	tmp = split_string(vars->input, var);
	free(var);
	if (!tmp)
		return (err_msg("Error", 1), 0);
	i = -1;
	j = 0;
	while (tmp[1][++i])
		if (ft_isalpha(tmp[1][i]))
			j++;
	var = ft_substr(tmp[1], 2, j);
	if (!var)
		return (err_msg("Error", 1), free_doubles(tmp), 0);
	cmd = ft_split(var, ' ');
	if (!path_finder(vars, var, cmd, 0))
		vars->output = NULL;
	free(tmp[1]);
	tmp[1] = ft_strdup(vars->output);
	append_doubles(&vars->input, tmp);
	return (1);
}

void	dolar_parse(t_vars *vars)
{
	int		i;
	char	**args;
	int		quote_type;

	i = -1;
	while (vars->input[++i])
	{
		if (vars->input[i] == '\"')
			quote_type = !quote_type;
		else if (quote_type == 0 && vars->input[i] == '\'')
			while (vars->input[++i] != '\'')
				;
		else if (vars->input[i] == '$')
		{
			if (vars->input[i + 1] == 40)
				exec_dollar_command(vars, i - 1);
			else
				env_find_dollar(vars, --i);
		}
	}
}

