
#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_vars
{
	int		id;
	char	*user_pwd;
	char	*input;
	char	*output;
	char	**input_parsed;
	char	**env;
	int		argc;
	int		exit_stat;
	int		origin_stdin;
	int		origin_stdout;
	int		file_opened;
	int		file_created;
}	t_vars;

int g_l;

//free and exit
void	err_msg(char *msg, int x);
void	free_doubles(char **str);
void	null_free(char **var);
void	null_free_void(void **var);
void	killer(t_vars *vars);
void	free_doubles2(void **str, int j);
//execute
int		handle_prompt(t_vars *vars, int condition);
int		path_finder(t_vars *vars, char *cmd, char **argv, int condition);
int		pipe_exec(char *path, t_vars *vars, char **argv, int condition);
int		pipe_parse(t_vars *vars);

int		marche(t_vars *vars, char **env, int condition);
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

//utils
int		reset_fds(t_vars *vars);
void	reset_vars(t_vars *vars);
int		is_quote(char c);
int		is_space(char c);
int		double_counter(char **str);
char	**split_string(char *src, char *key);
char	*strip(char *str);
void	append_doubles(char **dest, char **src, int condition);
int		dolar_parse(t_vars *vars);

//input and output
int		open_file(t_vars *vars, int i);
int		output_file(t_vars *vars, int i);
int		append_output(t_vars *vars, int i);
int		heredoc(t_vars *vars, int i);

//signals
int		wexitstatus(int status);
void	sig_c(int sig);
void	init_signals(void);
void	init_signals2(void);

#endif