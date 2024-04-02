#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	is_logic(int chr, int chr2)
{
	if (chr == '\0')
		return (0);
	if (chr2 == '\0')
		return (0);
	return ((chr == '&' && chr2 == '&')
			|| (chr == '|' && chr2 == '|'));
}

int	count_op(t_vars *vars)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (vars->input[++i])
		if (is_logic(vars->input[i], vars->input[i + 1]))
				count ++;
	return (count++);
}

int	exec_commands(char **commands, t_vars *vars)
{
	int		i;
	int		index;
	char	*tmp;
	t_vars	*child_vars;

	child_vars = malloc(sizeof(t_vars) * double_counter(commands));
	if (!child_vars)
		return (err_msg("Malloc error"), 0);
	i = -1;
	index = 0;
	while (commands[++i])
	{
		marche(&child_vars[index], vars->env, 0);
		if (i != 0)
		{
			tmp = ft_substr(commands[i], 1, ft_strlen(commands[i]));
			child_vars[index].input = strip(tmp);
		}
		else
			child_vars[index].input = strip(commands[i]);
		handle_prompt(&child_vars[index], 0);
		if (commands[i + 1] != NULL && commands[i + 1][0]\
		== '&' && child_vars[index].exit_stat)
			break ;
		if (commands[i + 1] != NULL && commands[i + 1][0]\
		== '|' && !child_vars[index].exit_stat)
			break ;
		index++;
	}
	return (1);
}

int	seek_operator(t_vars *vars)
{
	int		index;
	int		i;
	int		j;
	int		count;
	char	**commands;

	count = count_op(vars);
	if (count == 0)
		return (1);
	commands = malloc(sizeof(char *) * (count + 2));
	if (!commands)
		return (err_msg("Malloc error"), 0);
	i = 0;
	index = 0;
	while (vars->input[i] != '\0')
	{
		j = 0;
		int j = 0;
        while (vars->input[i + j] != '\0' && !is_logic(vars->input[i + j], vars->input[i + j + 1]))
            j++;
        commands[index] = ft_substr(vars->input, i, j);
        index++;
        i += j;
        if (vars->input[i] != '\0')
            i++;
	}
	commands[index] = NULL;
	exec_commands(commands, vars);
	free_doubles(commands);
	return (null_free(&vars->input), 1);
}
