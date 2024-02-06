
#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

int	new_cd(t_vars *vars, char *path_to_go)
{
	int		i;
	char	*old_path;
	char	*new_path;
	char	*tmp;

	if (!path_to_go)
		return (1);
	old_path = ft_strdup(getcwd(NULL, 0));
	if (chdir(path_to_go) != 0)
		return (perror("Error"), 0);
	new_path = getcwd(NULL, 0);
	i = find_in_env(vars, "PWD=");
	tmp = ft_strjoin("PWD=", new_path);
	if (i != -1)
		ft_strlcpy(vars->env[i], tmp, ft_strlen(tmp) + 1);
	free(new_path);
	free(tmp);
	i = find_in_env(vars, "OLDPWD");
	tmp = ft_strjoin("OLDPWD=", old_path);
	free(old_path);
	if (i != -1)
		ft_strlcpy(vars->env[i], tmp, ft_strlen(tmp) + 1);
	return (free(tmp), 1);
}
