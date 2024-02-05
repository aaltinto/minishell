#ifndef MINISHELL_H
#define MINISHELL_H

typedef struct s_vars
{
	char	*path;
	char	**command;
	char	**env;
}	t_vars;

void	err_msg(char *msg, int x);
void	free_doubles(char **str);

int		find_in_env(t_vars *vars, char *to_find);

int		new_cd(t_vars *vars, char *path_to_go);
void	new_pwd(t_vars *vars);

#endif