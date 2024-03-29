/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:52 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/24 17:54:07 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_vars
{
	int		hist;
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

int	g_l;

//free and exit
void	err_msg(char *msg);
void	free_doubles(char **str);
void	null_free(char **var);
void	killer(t_vars *vars);
void	free_doubles2(void **str, int j);
//execute
int		handle_prompt(t_vars *vars, int condition);
int		path_finder(t_vars *vars, char *cmd, char **argv);
int		pipe_exec(char *path, t_vars *vars, char **argv);
int		pipe_parse(t_vars *vars);

int		marche(t_vars *vars, char **env, int condition);
int		find_in_env(char **env, char *to_find);
char	*get_env(t_vars *vars, char *to_find);
int		set_env(t_vars *vars, char *to_find, char *to_set);
int		env_init(t_vars *vars, char **env);
char	**dup_env(t_vars *vars, char **to_dup);

//builtins
int		new_cd(t_vars *vars);
int		new_pwd(t_vars *vars);
void	echo(t_vars *vars);
int		new_export(t_vars *vars);
void	print_vars(t_vars *vars);
void	check_restore(t_vars *vars, int count);
int		unset(t_vars *vars, int del, int count);
void	new_env(t_vars *vars);

int		quote(t_vars *vars);
int		quote_pass(t_vars *vars, int i, char *quote_type, int *in_quotes);
int		re_init_env(t_vars *vars, int count, int del);

//utils
int		reset_fds(t_vars *vars);
void	reset_vars(t_vars *vars);
int		is_quote(char c);
int		is_space(char c);
int		double_counter(char **str);
char	**split_string(char *src, char *key);
char	*strip(char *tmp);
int		append_doubles(char **dest, char **src, int condition);

//parsing
int		parse(t_vars *vars, int count);
int		dolar_parse(t_vars *vars);
int		open_fds_parse(t_vars *vars);
int		exit_status(t_vars *vars, int i);
int		env_find_dollar(t_vars *vars, int i, int j);

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