
#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

int	path_finder(t_vars *vars, char *cmd, char **argv, int condition)
{
	char	**split_path;
	char	*this_is_the_way;
	char	*err;
	int		i;

	i = find_in_env(vars->env, "PATH=");
	split_path = ft_split(vars->env[i], ':');
	if (!split_path)
		return (err_msg("Error!", 1), 0);
	cmd = ft_strjoin("/", cmd);
	i = -1;
	while (split_path[++i])
	{
		this_is_the_way = ft_strjoin(split_path[i], cmd);
		if (access(this_is_the_way, F_OK) == 0)
			return (free_doubles(split_path), free(cmd),
				pipe_exec(this_is_the_way, vars, argv, condition), 1);
		free(this_is_the_way);
	}
	err = ft_strjoin("minishell: command not found: ", cmd);
	return (err_msg(err, 1), free(err), 0);
}

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
		return (free(tmp), free_doubles(user), NULL);
	free(tmp);
	tmp = ft_strjoin(user[1], "@ \e[1;92m");
	free_doubles(user);
	if (!tmp)
		return (free_doubles(pwd), NULL);
	i = double_counter(pwd);
	ret = ft_strjoin(tmp, pwd[i - 1]);
	free_doubles(pwd);
	free(tmp);
	if (!ret)
		return (NULL);
	tmp = ft_strjoin(ret, " $ \e[0m");
	return (free(ret), tmp);
}

int	main(int argc, char **argv, char **env)
{
	char	*pwd;
	t_vars	vars;

	if (argc != 1)
		return (err_msg("Error\nRun without arguments", 1), 1);
	printf("\e[1;33mMornin' Sunshine 🌞\n\e[0m");
	if (!env_init(&vars, env))
		return (err_msg("Env couldn't initialized", 1), 1);
	while (1)
	{
		pwd = find_w_dir(vars.env);
		if (!pwd)
			return (err_msg("Error!", 1), 1);
		vars.input = readline(pwd);
		free(pwd);
		if (handle_prompt(&vars) == 2)
			break ;
	}
}
