#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

void	new_env(t_vars *vars)
{
	int	i;

	i = -1;
	while (vars->env[++i])
		if (ft_strchr(vars->env[i], '=') != 0)
			printf("%s\n", vars->env[i]);
}
