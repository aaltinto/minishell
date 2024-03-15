#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

void	echo(t_vars *vars)
{
	int		i;

	if (!vars->input_parsed[1])
	{
		printf("\n");
		return ;
	}
	i = 0;
	if (ft_strncmp(vars->input_parsed[1], "-n", 3) == 0)
		i++;
	while (vars->input_parsed[++i])
	{
		ft_putstr_fd(vars->input_parsed[i], 1);
	}
	if (ft_strncmp(vars->input_parsed[1], "-n", 3) != 0)
		ft_putstr_fd("\n", 1);
}
