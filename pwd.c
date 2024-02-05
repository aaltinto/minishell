#include "minishell.h"
#include "libft/libft.h"

void	new_pwd(t_vars *vars)
{
	int	i;

	i = find_in_env(vars, "PWD");
	if (i == -1)
		return ;
	ft_putendl_fd(vars->env[i], 1);
}