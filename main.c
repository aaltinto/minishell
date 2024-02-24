
#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

void	command_exec(char *path, t_vars *vars)
{
	int	p_id;

	p_id = fork();
	if (p_id == 0)
		if (execve(path, vars->input_parsed, vars->env) == -1)
			perror("execute problem");
	waitpid(p_id, NULL, 0);
	free(path);
}

int	path_finder(t_vars *vars)
{
	char	**split_path;
	char	*this_is_the_way;
	char	*cmd;
	char	*err;
	int		i;

	i = find_in_env(vars->env, "PATH=");
	split_path = ft_split(vars->env[i], ':');
	if (!split_path)
		return (err_msg("Error!", 1), 0);
	cmd = ft_strjoin("/", vars->input_parsed[0]);
	i = -1;
	while (split_path[++i])
	{
		this_is_the_way = ft_strjoin(split_path[i], cmd);
		if (access(this_is_the_way, F_OK) == 0)
			return (free_doubles(split_path), free(cmd),
				command_exec(this_is_the_way, vars), 1);
		free(this_is_the_way);
	}
	err = ft_strjoin("minishell: command not found: ", vars->input_parsed[0]);
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
	tmp = getcwd(NULL, 0);
	pwd = ft_split(tmp, '/');
	free(tmp);
	i = double_counter(pwd);
	tmp = ft_strjoin(user[1], "@ \e[1;92m");
	free_doubles(user);
	ret = ft_strjoin(tmp, pwd[i - 1]);
	free_doubles(pwd);
	free(tmp);
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
	env_init(&vars, env);

	g_l = 0;
	while (1)
	{
		pwd = find_w_dir(vars.env);
		init_signals();
		vars.input = readline(pwd);
		init_signals2();
		free(pwd);
		if (handle_prompt(&vars) == 2)
			break ;
	}
}
