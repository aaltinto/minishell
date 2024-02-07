
#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include "libft/libft.h"

void	command_exec(char *path, char **command, t_vars *vars)
{
	int	p_id;

	p_id = fork();
	if (p_id == 0)
		if (execve(path, command, vars->env) == -1)
			perror("execute problem");
	waitpid(p_id, NULL, 0);
}

int	path_finder(t_vars *vars)
{
	char	**split_path;
	char	*this_is_the_way;
	char	**prompt;
	char	*cmd;
	int		i;

	i = find_in_env(vars, "PATH=");
	split_path = ft_split(vars->env[i], ':');
	if (!split_path)
		return (err_msg("Error!", 1), 0);
	prompt = ft_split(vars->input, ' ');
	cmd = ft_strjoin("/", prompt[0]);
	i = -1;
	while (split_path[++i])
	{
		this_is_the_way = ft_strjoin(split_path[i], cmd);
		if (access(this_is_the_way, F_OK) == 0)
			return (free_doubles(split_path),
				command_exec(this_is_the_way, prompt, vars), 1);
		free(this_is_the_way);
	}
	return (err_msg(ft_strjoin("minishell: command not found: ", prompt[0]),
			1), 0);
}

char	*find_w_dir(char **env)
{
	int		i;
	char	**user;
	char	**pwd;
	char	*tmp;
	char	*ret;

	i = -1;
	while (env[++i])
		if (ft_strncmp("USER=", env[i], 5) == 0)
			break ;
	user = ft_split(env[i], '=');
	pwd = ft_split(getcwd(NULL, 0), '/');
	i = 0;
	while (pwd[i])
		++i;
	tmp = ft_strjoin(user[1], "@ \e[1;92m");
	free_doubles(user);
	ret = ft_strjoin(tmp, pwd[i - 1]);
	free(tmp);
	tmp = ft_strjoin(ret, " $ \e[0m");
	return (free_doubles(pwd), free(ret), tmp);
}

int	main(int argc, char **argv, char **env)
{
	char	*pwd;
	t_vars	vars;

	if (argc != 1)
		return (err_msg("Error\nRun without arguments", 1), 1);
	env_init(&vars, env);
	while (1)
	{
		pwd = find_w_dir(vars.env);
		vars.input = readline(pwd);
		add_history(vars.input);
		if (handle_prompt(&vars) == 2)
			break ;
		free(vars.input);
		free(pwd);
	}
}
