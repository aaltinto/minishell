
#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include "libft/libft.h"

#define EXIT 5

char	*get_path(char **env, char *to_find, int len)
{
	int 	i;
	char	*ret;

	i = -1;
	while(env[++i])
		if (ft_strncmp(to_find, env[i], len) == 0)
			break ;
	ret = ft_strdup(env[i]);
	return(ret);
}

int	is_builtin(t_vars *vars, char *input)
{
	char	**command;

	command = ft_split(input, ' ');
	if (ft_strncmp(command[0], "cd", 3) == 0)
		return (new_cd(vars, command[1]));
	if (ft_strncmp(command[0], "pwd", 4) == 0)
		return (new_pwd(vars), 1);
	return (0);
}

int	something_familiar(char *input, t_vars *vars)
{
	if (ft_strncmp("", input, 1) == 0)
		return (1);
	else if (ft_strncmp("exit", input, 5) == 0)
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", input, 3) == 0)
		return (printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars, input))
		return (1);
	else
		return (0);

}
char	*find_the_way(char **path, char *input, t_vars *vars)
{
	int		i;
	char	*this_is_the_way;
	char	*cmd;

	vars->command = ft_split(input, ' ');
	if (!vars->command)
		return (err_msg("Error!", 1), NULL);
	cmd = ft_strjoin("/", vars->command[0]);
	i = -1;
	while(path[++i])
	{
		this_is_the_way = ft_strjoin(path[i], cmd);

		if (access(this_is_the_way, F_OK) == 0)
			return (free_doubles(path), this_is_the_way);
		free(this_is_the_way);
	}
	return (NULL);
}

char	*path_finder(char **env, char *input, t_vars *vars)
{
	char	**split_path;
	char	*path;

	path = get_path(env, "PATH=", 5);
	split_path = ft_split(path, ':');
	if (!split_path)
		return(err_msg("Error!", 1), NULL);
	return (find_the_way(split_path, input, vars));
}

void	command_exec(char *path, t_vars *vars, char **env)
{
	int	p_id;

	p_id = fork();
	if (p_id == 0)
		if (execve(path, vars->command, env) == -1)
			perror("execute problem");
	waitpid(p_id, NULL, 0);
}

int	handle_prompt(char *input, char **env, t_vars *vars)
{
	int	ret;
	char	*this_is_the_way;

	ret = something_familiar(input, vars);
	if (ret)
		return (ret);
	this_is_the_way = path_finder(env, input, vars);
	if (!this_is_the_way)
		return (err_msg("command not found: ", 0), err_msg(vars->command[0], 1), 1);
	command_exec(this_is_the_way, vars, env);
	return (1);
}

char	*find_w_dir(char **env)
{
	int	i;
	char	**user;
	char	**pwd;
	char	*tmp;
	char	*ret;

	i = -1;
	while(env[++i])
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

int	env_init(t_vars *vars, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	vars->env = malloc(sizeof(char *) * (i + 1));
	if (!vars->env)
		return (free_doubles(vars->env), 0);
	i = -1;
	while (env[++i])
		vars->env[i] = ft_strdup(env[i]);
	return (printf("\e[1;33mMornin' Sunshine 🌞\n\e[0m"), 1);
}

int	main(int argc, char **argv, char **env)
{
	char *input;
	char *pwd;
	t_vars	vars;

	if (argc != 1)
		return (err_msg("Error\nRun without arguments", 1), 1);
	env_init(&vars, env);
	while (1)
	{
		pwd = find_w_dir(vars.env);
		input = readline(pwd);
		add_history(input);
		if (handle_prompt(input, vars.env, &vars) == 2)
			break ;
		free(input);
		free(pwd);
	}
}
