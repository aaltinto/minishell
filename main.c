/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:46 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/16 16:15:50 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdlib.h>

int	g_l;

char	*find_work_dir(char **env)
{
	char	**split;
	char	*pwd;
	char	*ret;
	int		i;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("getcwd"), ft_strdup(".. "));
	i = find_in_env(env, "HOME=");
	if (i != -1 && ft_strncmp(pwd, env[i] + 5, ft_strlen(env[i])) == 0)
		ret = ft_strdup(" ~");
	else if (ft_strncmp(pwd, "/", 2) == 0)
		ret = ft_strdup(" /");
	else
	{
		split = ft_split(pwd, '/');
		if (!split)
			return (null_free(&pwd), NULL);
		i = double_counter(split);
		ret = ft_strjoin(" ", split[i -1]);
		free_doubles(split);
	}
	return (null_free(&pwd), ret);
}

int	prompter(char **env, t_vars *ret)
{
	int		i;
	char	**vars;

	vars = malloc(sizeof(char *) * 6);
	if (!vars)
		return (0);
	i = find_in_env(env, "USER=");
	if (i == -1)
		vars[0] = ft_strdup("user");
	else
		vars[0] = ft_strdup(env[i] + 5);
	vars[1] = ft_strdup("@");
	vars[2] = find_work_dir(env);
	vars[3] = ft_strdup(" $ ");
	if (!vars[0] || !vars[1] || !vars[2] || !vars[3])
		return (err_msg("strdup error"), free_doubles(vars), 1);
	vars[4] = NULL;
	return (append_doubles(&ret->user_pwd, vars, 0), free_doubles(vars), 1);
}

int	opening_ceremony(t_vars *vars)
{
	int	i;
	int	count;

	count = double_counter(vars->env);
	i = find_in_env(vars->env, "OLDPWD=");
	//printf("\e[1;33mMornin' Sunshine 🌞\n\e[0m");
	if (i != -1)
		return (null_free(&vars->env[i]),
			re_init_env(vars, (count), 1), 1);
	return (1);
}

int	marche(t_vars *vars, char **env, int condition)
{
	if (!env_init(vars, env))
		return (err_msg("Env couldn't initialized"), 0);
	vars->input = NULL;
	vars->input_parsed = NULL;
	vars->output = NULL;
	vars->user_pwd = NULL;
	vars->file_created = 0;
	vars->file_opened = 0;
	vars->exit_stat = 0;
	vars->hist = 0;
	vars->bonus = 0;
	if (condition)
		return (opening_ceremony(vars));
	return (1);
}

void	sig_c(int sig)
{
	(void)sig;
	if (!g_l)
	{
		printf("\n");
		rl_on_new_line();
		//rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_l == 42)
	{
		printf("\n");
		exit(EXIT_FAILURE);
	}
}

void	sig_backslash(int sig)
{
	(void)sig;
	printf("Quit: 3\n");
}

void	init_signals(void)
{
	g_l = 0;
	signal(SIGINT, sig_c);
	signal(SIGQUIT, SIG_IGN);
}

void	init_signals2(void)
{
	g_l = 1;
	signal(SIGINT, sig_c);
	signal(SIGQUIT, sig_backslash);
}

int	main(int argc, char **argv, char **env)
{
	t_vars	vars;

	(void)argv;
	if (argc != 1)
		return (err_msg("Error\nRun without arguments"), 1);
	if (!marche(&vars, env, 1))
		exit (EXIT_FAILURE);
	while (1)
	{
		reset_vars(&vars);
		if (!prompter(vars.env, &vars) || !vars.user_pwd)
			return (err_msg("Prompter error!"), 1);
		init_signals();
		vars.input = readline(vars.user_pwd);
		init_signals2();
		null_free(&vars.user_pwd);
		if (handle_prompt(&vars, 1) == 2)
			break ;
	}
	exit_setter(&vars);
	killer(&vars);
	return (vars.exit_stat);
}
