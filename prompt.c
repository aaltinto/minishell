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

void	re_create_input(t_vars *vars, char *var, char **args, int i)
{
	int		j;
	int		dstsize;
	char	*ret;
	char	**tmp;

	dstsize = ft_strlen(vars->input) + ft_strlen(var) + 1;
	ret = malloc(dstsize);
	j = -1;
	while (args[++j])
	{
		if (j == i)
		{
			tmp = ft_split(args[j], '$');
			if (!var)
			{
				ft_strlcat(ret, tmp[0], dstsize);
				free_doubles(tmp);
				continue ;
			}
			if (tmp[1] != NULL)
				ft_strlcat(ret, tmp[0], dstsize);
			free_doubles(tmp);
			ft_strlcat(ret, var, dstsize);
			ft_strlcat(ret, " ", dstsize);
			continue ;
		}
		ft_strlcat(ret, args[j], dstsize);
		ft_strlcat(ret, " ", dstsize);
	}
	free(vars->input);
	vars->input = ft_strdup(ret);
	printf("%s\n", vars->input);
}

int	env_find_dollar(t_vars *vars, int i, char **args)
{
	int		j;
	char	*var;
	char	**str;
	char	**tmp;

	j = 0;
	while (vars->input[++i] && (vars->input[i] != ' ' && vars->input[i] != '\''
			&& vars->input[i] != '\"'))
		j++;
	var = ft_substr(vars->input, i - j, j);
	printf("var= %s\nimput[i]= %c\n", var, vars->input[i + 1]);
	i = -1;
	while (args[++i] && !ft_strchr(args[i], '$'))
		;
	tmp = ft_split(args[i], '$');
	j = find_in_env(vars->env, var);
	free(var);
	if (j != -1)
		str = ft_split(vars->env[j], '=');
	if (j == -1)
		return (re_create_input(vars, NULL, args, i), 1);
	re_create_input(vars, str[1], args, i); 
	return (free_doubles(str), free_doubles(tmp), 1);
}

void	dolar_parse(t_vars *vars)
{
	int		i;
	char	**args;
	int		quote_type;

	args = ft_split(vars->input, ' ');
	for(int i = 0; args[i]; i++)
		printf("%s ", args[i]);
	printf("\n");
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
			env_find_dollar(vars, i, args);
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
