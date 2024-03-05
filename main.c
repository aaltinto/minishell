
#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <errno.h>

char	*find_w_dir(char **env)
{
	int		i;
	char	**user;
	char	**pwd;
	char	*tmp;
	char	*ret;

	i = find_in_env(env, "USER=");
	user = ft_split(env[i], '=');
	if (!user)
		return (NULL);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (free_doubles(user), NULL);
	pwd = ft_split(tmp, '/');
	if (!pwd)
		return (null_free(&tmp), free_doubles(user), NULL);
	null_free(&tmp);
	tmp = ft_strjoin(user[1], "@ \e[1;92m");
	free_doubles(user);
	if (!tmp)
		return (free_doubles(pwd), NULL);
	i = find_in_env(env, "HOME=");
	user = ft_split(env[i], '/');
	if (!pwd)
		return (null_free(&ret), NULL);
	i = double_counter(pwd);
	if (i == 0)
		ret = ft_strjoin(tmp, "/");
	else if (ft_strncmp(pwd[i - 1], user[double_counter(user) -1],
			ft_strlen(user[1])) == 0)
		ret = ft_strjoin(tmp, "~");
	else
		ret = ft_strjoin(tmp, pwd[i - 1]);
	free_doubles(pwd);
	null_free(&tmp);
	if (!ret)
		return (NULL);
	tmp = ft_strjoin(ret, " $ \e[0m");
	if (!pwd)
		return (null_free(&ret), free_doubles(pwd), NULL);
	return (null_free(&ret), tmp);
}

void	reset_vars(t_vars *vars)
{
	vars->file_created = 0;
	vars->file_opened = 0;
	null_free(&vars->input);
	null_free(&vars->output);
	free_doubles(vars->input_parsed);
}

int	marche(t_vars *vars, char **env)
{
	int		i;
	char	**tmp;

	if (!env_init(vars, env))
		return (err_msg("Env couldn't initialized", 1), 0);
	// i = find_in_env(env, "HOME=");
	// tmp = ft_split(vars->env[i], '=');
	// if (!tmp)
	// 	return (err_msg("ft_split error!", 1), 0);
	// chdir(tmp[1]);
	// if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
	// 	return (perror("dup2"), 0);
	vars->input = NULL;
	vars->input_parsed = NULL;
	vars->output = NULL;
	vars->file_created = 0;
	vars->file_opened = 0;
	printf("\e[1;33mMornin' Sunshine 🌞\n\e[0m");
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	char	*pwd;
	t_vars	vars;

	if (argc != 1)
		return (err_msg("Error\nRun without arguments", 1), 1);
	if (!marche(&vars, env))
		exit (EXIT_FAILURE);
	while (1)
	{
		reset_vars(&vars);
		pwd = find_w_dir(vars.env);
		if (!pwd)
			return (err_msg("Error!", 1), 1);
		vars.input = readline(pwd);
		null_free(&pwd);
		if (dup2(vars.origin_stdin, STDIN_FILENO) == -1)
			return (perror("dup2"), 0);
		if (handle_prompt(&vars) == 2)
			break ;
	}
	return (1);
}
