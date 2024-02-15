#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

char	**dup_env(t_vars *vars, char **to_dup)
{
	int		j;
	int		i;
	char	**new_env;

	i = double_counter(to_dup);
	new_env = malloc(sizeof(char *) * (i + vars->argc + 1));
	i = -1;
	while (to_dup[++i])
	{
		new_env[i] = ft_strdup(to_dup[i]);
		if (!new_env[i])
			return (err_msg("Error", 1), NULL);
	}
	new_env[i] = NULL;
	return (new_env);
}

void	print_vars(t_vars *vars)
{
	char	**exports;
	int		i;

	i = -1;
	while (vars->env[++i])
	{
		exports = ft_split(vars->env[i], '=');
		if (!exports[1])
			printf("declare -x %s\n", exports[0]);
		else
			printf("declare -x %s=\"%s\"\n", exports[0], exports[1]);
		free_doubles(exports);
	}
}

int	new_export(t_vars *vars)
{
	int		i;
	int		i2;
	int		j;
	char	**new_env;

	if (!vars->input_parsed[1])
		return (print_vars(vars), 1);
	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (2);
	i2 = double_counter(new_env) - 1;
	i = 0;
	while (vars->input_parsed[++i])
	{
		new_env[++i2] = ft_strdup(vars->input_parsed[i]);
		if (!new_env[i2])
			return (err_msg("Error", 1), 2);
	}
	new_env[++i2] = NULL;
	free_doubles(vars->env);
	return (env_init(vars, new_env), 1);
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
	new_env = malloc(sizeof(char *) * (count - del + 1));
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