
#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_vars
{
	char	*input;
	char	**env;
}	t_vars;

void	err_msg(char *msg, int x);
void	free_doubles(char **str);

int		handle_prompt(t_vars *vars);
int		path_finder(t_vars *vars);
void	command_exec(char *path, char **command, t_vars *vars);

int		find_in_env(t_vars *vars, char *to_find);
int		env_init(t_vars *vars, char **env);

int		new_cd(t_vars *vars);
void	new_pwd(t_vars *vars);
void	echo(t_vars *vars);

int		quote(t_vars *vars);

#endif