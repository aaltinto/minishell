
#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	find_in_env(t_vars *vars, char *to_find)
{
	int	i;

	i = -1;
	while (vars->env[++i])
		if (ft_strncmp(vars->env[i], to_find, ft_strlen(to_find)) == 0)
			return (i);
	return (-1);
}

int	env_init(t_vars *vars, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	vars->env = malloc(sizeof(char *) * (i + 1));
	if (!vars->env)
		return (free_doubles(vars->env), 0);
	i = -1;
	while (env[++i])
		vars->env[i] = ft_strdup(env[i]);
	i = -1;
	return (ft_putstr_fd("\e[1;33mMornin' Sunshine 🌞\n\e[0m", 1), 1);
}
