#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	is_builtin(t_vars *vars)
{
	if (ft_strncmp(vars->input_parsed[0], "cd", 2) == 0)
		return (new_cd(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "pwd", 3) == 0)
		return (new_pwd(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "echo", 4) == 0)
		return (echo(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "export", 5) == 0)
		return (new_export(vars));
	if (ft_strncmp(vars->input_parsed[0], "env", 3) == 0)
		return (new_env(vars), 1);
	if (ft_strncmp(vars->input_parsed[0], "unset", 5) == 0)
		return (unset(vars), 1);
	return (0);
}

int	something_familiar(t_vars *vars)
{
	if (vars->input_parsed == NULL)
		return (1);
	if (ft_strncmp("exit", vars->input_parsed[0], 5) == 0)
		return (printf("See ya 🫡\n"), 2);
	else if (ft_strncmp("hi", vars->input_parsed[0], 3) == 0)
		return (printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars))
		return (1);
	else
		return (0);
}

int	reset_fds(t_vars *vars)
{
	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	if (vars->file_opened)
		if (dup2(vars->origin_stdin, STDIN_FILENO) == -1)
			return (perror("dup2"), 0);
	return (1);
}

int pipe_parse(t_vars *vars) {
    int i = -1;
    int pipe_count = 0;

    while (vars->input[++i])
        if (vars->input[i] == '|')
            pipe_count++;
    if (pipe_count == 0)
        return (1);
    int num_commands = pipe_count + 1; // Number of commands

    int pipes[pipe_count][2]; // Array to store pipes

    char **argv = ft_split(vars->input, '|');
    // Create pipes
    for (int i = 0; i < num_commands - 1; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    pid_t pid[num_commands - 1];
    // Loop to create child processes
    for (int i = 0; i < num_commands; ++i) {
        pid[i] = fork();
        if (pid[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid[i] == 0) { // Child process
            // Close unused pipe ends
            if (i != 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO); // Redirect stdin from the previous pipe
                close(pipes[i - 1][0]); // Close read end of previous pipe
            }
            if (i != num_commands - 1) {
                close(pipes[i][0]); // Close read end of current pipe
                dup2(pipes[i][1], STDOUT_FILENO); // Redirect stdout to the current pipe
            }

            // Close all pipe ends
            for (int j = 0; j < num_commands - 1; ++j) {
                if (j != i - 1)
                    close(pipes[j][0]);
                if (j != i)
                    close(pipes[j][1]);
            }

            // Execute the command
            char **cmd = ft_split(argv[i], ' ');
            int env_path_index = find_in_env(vars->env, "PATH=");
            char **paths = ft_split(vars->env[env_path_index] + 5, ':');
            char *cmd_path;
            for (int i = 0; paths[i]; i++) {
                char *this_the_way = ft_strjoin(paths[i], "/");
                cmd_path = ft_strjoin(this_the_way, cmd[0]);

                if (access(cmd_path, F_OK) == 0) {
                    ft_putendl_fd(cmd_path, 2);
                    if (execve(cmd_path, cmd, vars->env) != 0)
                        perror("execve");
                }
                free(this_the_way);
                free(cmd_path);
            }
            //execvp(argv[i], &argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipe ends in parent process
    for (int i = 0; i < num_commands - 1; ++i) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; ++i) {
        waitpid(pid[i], NULL, 0);
    }
    null_free(&vars->input);
    return 1;
}

int	handle_prompt(t_vars *vars)
{
	int		ret;

	if (!vars->input)
		return (2);
	if (ft_strncmp("", vars->input, 2) == 0)
		return (1);
	quote(vars);
	add_history(vars->input);
	dolar_parse(vars);
	if (!vars->input)
		return (reset_fds(vars));
	if (!pipe_parse(vars))
		return (err_msg("Error", 1), 1);
	write(1, "b\n", 2);
	if (!parse(vars, 0))
		return (0);
	ret = something_familiar(vars);
	if (!ret)
		path_finder(vars, vars->input_parsed[0], vars->input_parsed, 1);
	if (!reset_fds(vars))
		return (0);
	return (ret);
}
