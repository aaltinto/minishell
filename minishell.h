
#ifndef MINISHELL_H
# define MINISHELL_H

#define MAX_BUFFER 1024
typedef struct s_vars
{
	char	*input;
	char	**input_parsed;
	char	**env;
}	t_vars;

#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include "libft/libft.h"

void	err_msg(char *msg, int x);
void	free_doubles(char **str);

int		handle_prompt(t_vars *vars);
int		path_finder(t_vars *vars);
void	command_exec(char *path, t_vars *vars);

int		find_in_env(t_vars *vars, char *to_find);
int		env_init(t_vars *vars, char **env);

int		new_cd(t_vars *vars);
void	new_pwd(t_vars *vars);
void	echo(t_vars *vars);
int		new_export(t_vars *vars);
void	new_env(t_vars *vars);

int		quote(t_vars *vars);
int		parse(t_vars *vars);

int		is_quote(char c);
int		is_space(char c);

#endif