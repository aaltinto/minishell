#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

int	count_vars(t_vars *vars)
{
	int		i;
	int		j;
	int		new_var;

	new_var = 0;
	i = 0;
	while (vars->input_parsed[++i])
	{
		j = -1;
		while (vars->input_parsed[i][++j])
			if (vars->input_parsed[i][j] == '=')
				new_var++;
	}
	return (new_var);
}

char	**dup_env(t_vars *vars, char **to_dup)
{
	int		j;
	int		i;
	int		count;
	char	**new_env;

	i = 0;
	while (to_dup[i])
		i++;
	count = count_vars(vars);
	if (!count)
		return (NULL);
	new_env = (char **)malloc(i + count);
	i = -1;
	while (to_dup[++i])
		new_env[i] = ft_strdup(to_dup[i]);
	new_env[i] = NULL;
	return (new_env);
}

int	new_export(t_vars *vars)
{
	int		i;
	int		i2;
	int		j;
	char	**new_env;

	if (!vars->input_parsed[1])
		return (1);
	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (2);
	for (int i = 0; new_env[i];i++)
		printf("%s\n", new_env[i]);
	i2 = 0;
	while (new_env[i2])
		i2++;
	i = 0;
	while (vars->input_parsed[++i])
	{
		j = -1;
		while (vars->input_parsed[i][++j])
		{
			if (vars->input_parsed[i][j] == '=')
			{
				new_env[i2] = ft_strdup(vars->input_parsed[i]);
				if (!new_env[i2])
					return (err_msg("Error", 1), 2);
				i++;
				i2++;
				break ;
			}
		}
	}
	new_env[i2] = NULL;
	free_doubles(vars->env);
	dup_env(vars, new_env);
	free_doubles(new_env);
	return (1);
}
