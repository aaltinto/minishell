#include "../minishell.h"
#include <stdio.h>

void	new_env(t_vars *vars)
{
	int	i;

	i = -1;
	while (vars->env[++i])
		printf("%s\n", vars->env[i]);
}
