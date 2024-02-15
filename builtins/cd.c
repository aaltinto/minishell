
#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

int	new_cd(t_vars *vars)
{
	int		i;
	char	*old_path;
	char	*new_path;
	char	*tmp;

	old_path = getcwd(NULL, 0);
	if (chdir(vars->input_parsed[1]) != 0)
		return (perror("Error"), 0);
	new_path = getcwd(NULL, 0);
	i = find_in_env(vars->env, "PWD=");
	tmp = ft_strjoin("PWD=", new_path);
	free(new_path);
	if (i != -1)
	{
		free(vars->env[i]);
		vars->env[i] = ft_strdup(tmp);
	}
	free(tmp);
	i = find_in_env(vars->env, "OLDPWD");
	tmp = ft_strjoin("OLDPWD=", old_path);
	free(old_path);
	if (i != -1)
	{
		free(vars->env[i]);
		vars->env[i] = ft_strdup(tmp);
	}
	return (free(tmp), 1);
}
