#include "../minishell.h"
#include "../libft/libft.h"

void	new_pwd(t_vars *vars)
{
	int		i;
	char	**splitted;

	i = find_in_env(vars->env, "PWD");
	if (i == -1)
		return ;
	splitted = ft_split(vars->env[i], '=');
	ft_putendl_fd(splitted[1], 1);
}
