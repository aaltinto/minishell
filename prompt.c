#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	is_builtin(t_vars *vars)
{
	if (ft_strncmp(vars->input_parsed[0], "cd", 2) == 0)
		return (new_cd(vars));
	if (ft_strncmp(vars->input_parsed[0], "pwd", 3) == 0)
		return (new_pwd(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "echo", 4) == 0)
		return (echo(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "export", 5) == 0)
		return (new_export(vars));
	if (ft_strncmp(vars->input_parsed[0], "env", 3) == 0)
		return (new_env(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "unset", 5) == 0)
		return (unset(vars), 1);
	return (0);
}

int	something_familiar(t_vars *vars)
{
	if (ft_strncmp("exit", vars->input_parsed[0], 4) == 0)
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", vars->input_parsed[0], 2) == 0)
		return (printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars))
		return (1);
	else
		return (0);
}

char	**split_string(char *src, char *key)
{
	char	**result;
	char	*before;
	char	*after;
	char	*found;	

	result = malloc(3 * sizeof(char *));
	if (result == NULL)
		return (err_msg("Memory allocation failed\n", 1), NULL);
	found = ft_strnstr(src, key, ft_strlen(src));
	if (found != NULL)
	{
		before = src;
		*found = '\0';
		after = found + ft_strlen(key);
		*after = '\0';
	}
	if (!before)
		result[0] = NULL;
	else
		result[0] = ft_strdup(before);
	if (!key)
		result[1] = NULL;
	else
	result[1] = ft_strdup(key);
	if (!after)
		result[2] = NULL;
	else
		result[2] = ft_strdup(after);
	return (result);
}

int	env_find_dollar(t_vars *vars, int i, char **args)
{
	int		j;
	int		len;
	char	*var;
	char	**tmp;
	char	**original;

	original = vars->env;
	j = 0;
	while (vars->input[++i] && (vars->input[i] != ' ' && vars->input[i] != '\''
			&& vars->input[i] != '\"'))
		j++;
	var = ft_substr(vars->input, i - j, j);
	tmp = split_string(vars->input, var);
	j = find_in_env(vars->env, var + 1);
	free(var);
	free(tmp[1]);
	tmp[1] = ft_strdup(ft_strchr(vars->env[j], '=') + 1);
	vars->env = original;
	printf("%s\n", tmp[1]);
	len = 0;
	i = -1;
	while (++i < 3)
		if (tmp[i])
			len += ft_strlen(tmp[i]);
	free(vars->input);
	vars->input = malloc(len + 1);
	i = -1;
	while (++i <= 2)
		if (tmp[i])
			ft_strlcat(vars->input, tmp[i], len + 1);
	printf("input = %s\n", vars->input);
	free_doubles(tmp);
	return (1);
}

void	dolar_parse(t_vars *vars)
{
	int		i;
	char	**args;
	int		quote_type;

	args = ft_split(vars->input, ' ');
	quote_type = 0;
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
			env_find_dollar(vars, --i, args);
		}
	}
	free_doubles(args);
}

int	handle_prompt(t_vars *vars)
{
	int		ret;

	if (ft_strncmp("", vars->input, 2) == 0)
		return (free(vars->input), 1);
	quote(vars);
	add_history(vars->input);
	dolar_parse(vars);
	arg_counter(vars);
	parse(vars, 0);
	ret = something_familiar(vars);
	if (ret)
		return (ret);
	path_finder(vars);
	return (free_doubles(vars->input_parsed), 1);
}
