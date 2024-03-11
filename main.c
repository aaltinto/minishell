
#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

//[username][@][color][dir_name][$][reset_color]
char	*find_w_dir(char **env, t_vars *ret)
{
	int		i;
	char	**vars;
	char	**split;
	char	*pwd;

	vars = malloc(sizeof(char *) * 6);
	if (!vars)
		return (NULL);
	i = find_in_env(env, "USER=");
	vars[0] = ft_strdup(env[i] + 5);
	if (!vars[0])
		return (free_doubles(vars), NULL);
	vars[1] = ft_strdup("@\e[1;92m");
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (free_doubles(vars), NULL);
	i = find_in_env(env, "HOME=");
	if (ft_strncmp(pwd, env[i] + 5, ft_strlen(pwd)) == 0)
		vars[2] = ft_strdup(" ~");
	else
	{
		split = ft_split(pwd, '/');
		if (!split)
			return (null_free(&pwd), free_doubles(vars), NULL);
		i = double_counter(split);
		vars[2] = ft_strdup(split[i -1]);
		free_doubles(split);
	}
	null_free(&pwd);
	vars[3] = ft_strdup(" $ \e[0m");
	vars[4] = NULL;
	append_doubles(&ret->user_pwd, vars, 0);
	free_doubles(vars);
	return ("OK");
}

void	reset_vars(t_vars *vars)
{
	vars->file_created = 0;
	vars->file_opened = 0;
	null_free(&vars->input);
	null_free(&vars->output);
	free_doubles(vars->input_parsed);
	reset_fds(vars);
	vars->input_parsed = NULL;
}

int	marche(t_vars *vars, char **env, int condition)
{
	int		i;
	char	**tmp;

	if (!env_init(vars, env))
		return (err_msg("Env couldn't initialized", 1), 0);
	if (condition)
	{
		i = find_in_env(env, "HOME=");
		tmp = ft_split(vars->env[i], '=');
		if (!tmp)
			return (err_msg("ft_split error!", 1), 0);
		chdir(tmp[1]);
		free_doubles(tmp);
		printf("\e[1;33mMornin' Sunshine 🌞\n\e[0m");
	}
	vars->input = NULL;
	vars->input_parsed = NULL;
	vars->output = NULL;
	vars->user_pwd = NULL;
	vars->exit_stat = 0;
	vars->file_created = 0;
	vars->file_opened = 0;
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	char	*pwd;
	t_vars	vars;

	if (argc != 1)
		return (err_msg("Error\nRun without arguments", 1), 1);
	vars.id = 1;
	if (!marche(&vars, env, 1))
		exit (EXIT_FAILURE);
	while (1)
	{
		reset_vars(&vars);
		find_w_dir(vars.env, &vars);
		if (!vars.user_pwd)
			return (err_msg("Error!", 1), 1);
		init_signals();
		vars.input = readline(vars.user_pwd);
		init_signals2();
		null_free(&vars.user_pwd);
		if (handle_prompt(&vars, 1) == 2)
			break ;
	}
	killer(&vars);
	return (42);
}
