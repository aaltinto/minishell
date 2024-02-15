
#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	env_init(t_vars *vars, char **env)
{
	int	i;

	i = double_counter(env);
	vars->env = malloc(sizeof(char *) * (i + 1));
	if (!vars->env)
		return (0);
	i = -1;
	while (env[++i])
		vars->env[i] = ft_strdup(env[i]);
	vars->env[i] = NULL;
	return (1);
}
