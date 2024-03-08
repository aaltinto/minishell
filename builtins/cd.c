
#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	new_cd(t_vars *vars)
{
	int		i;
	char	*old_path;
	char	*new_path;
	char	*tmp;
	char	**home_path;

	i = find_in_env(vars->env, "HOME=");
	old_path = getcwd(NULL, 0);
	home_path = ft_split(vars->env[i], '=');
	if (!home_path)
		return (err_msg("Error", 1), free(old_path), 0);
	if (ft_strncmp("/", old_path, 2) == 0)
		return (null_free(&old_path), 0);
	if (chdir(vars->input_parsed[1]) != 0)
		return (perror("minishell: cd"), 0);
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
