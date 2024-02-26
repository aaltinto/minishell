
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "minishell.h"
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef struct s_vars
{
	char	*input;
	char	*output;
	char	**input_parsed;
	char	**env;
	int		argc;
}	t_vars;


void	err_msg(char *msg, int x);
void	free_doubles(char **str);

int		handle_prompt(t_vars *vars);
int		path_finder(t_vars *vars, char *cmd, char **argv, int condition);

int		find_in_env(char **env, char *to_find);
int		env_init(t_vars *vars, char **env);

//builtins
int		new_cd(t_vars *vars);
void	new_pwd(t_vars *vars);
void	echo(t_vars *vars);
int		new_export(t_vars *vars);
void	unset(t_vars *vars);
void	new_env(t_vars *vars);

int		quote(t_vars *vars);
int		parse(t_vars *vars, int count);

int		is_quote(char c);
int		is_space(char c);
int		double_counter(char **str);
char	**split_string(char *src, char *key);
void	append_doubles(char **dest, char **src);
void	dolar_parse(t_vars *vars);

int		pipe_exec(char *path, t_vars *vars, char **argv, int condition);

#endif