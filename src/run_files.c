/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 20:05:55 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/02 20:06:34 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include "../gnl/get_next_line.h"

static int	open_input(char **argv)
{
	int		len;
	int		fd;
	char	*msg;

	msg = ft_strjoin("minishell can't open ", argv[1]);
	len = ft_strlen(argv[1]);
	if (argv[1][len - 1] != 'h' || argv[1][len - 2] != 's'
		|| argv[1][len - 3] != '.')
		return (err_msg(msg), null_free(&msg), -1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror(msg), null_free(&msg), -1);
	return (null_free(&msg), fd);
}

static int	fix_input(char *input, char **env)
{
	char	**splited;
	t_vars	*vars;
	int		count;
	int		i;

	splited = ft_split(input, '\n');
	if (!splited)
		return (err_msg("Split error"), 1);
	count = double_counter(splited);
	vars = malloc(sizeof(t_vars) * count);
	i = -1;
	while (++i < count)
	{
		if (marche(&vars[i], env, 0))
			return (free_doubles(splited), free(vars), 1);
		vars[i].input = ft_strdup(splited[i]);
		handle_prompt(&vars[i], 0);
		null_free(&vars[i].input);
		free_doubles(vars[i].env);
		free_doubles(vars[i].input_parsed);
	}
	i = vars[count -1].exit_stat;
	return (free(vars), free_doubles(splited), i);
}

int	run_files(int ac, char **argv, char **env)
{
	char	*input;
	int		ret;

	if (ac > 2)
		return (err_msg("Too many arguments"), 1);
	input = get_next_line(open_input(argv));
	if (!input)
		return (1);
	ret = fix_input(input, env);
	null_free(&input);
	return (ret);
}