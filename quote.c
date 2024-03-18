/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:08 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/15 16:13:09 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <readline/readline.h>

void	wait_close(int quote_check, int type, t_vars *vars)
{
	char	*new_input;
	char	*tmp;
	int		i;

	while (quote_check % 2 == 0)
	{
		new_input = readline("> ");
		if (!new_input)
		{
			write(1, "\n", 1);
			err_msg("Quote error");
			vars->hist = -1;
			null_free(&vars->input);
			return ;
		}
		i = -1;
		while (new_input[++i])
		{
			if (type && new_input[i] == '\"')
				quote_check++;
			if (!type && new_input[i] == '\'')
				quote_check++;
		}
		tmp = ft_strjoin(vars->input, "\n");
		null_free(&vars->input);
		vars->input = ft_strjoin(tmp, new_input);
		null_free(&tmp);
		null_free(&new_input);
	}
	if (!quote(vars))
		return ;
}

int	quote(t_vars *vars)
{
	int	i;
	int	in_quote;

	in_quote = 0;
	i = -1;
	while (vars->input[++i])
	{
		if (vars->input[i] == '\"' && in_quote != 2)
		{
			if (!in_quote)
				in_quote = 1;
			else if (in_quote == 1)
				in_quote = 0;
		}
		else if (vars->input[i] == '\'' && in_quote != 1)
		{
			if (!in_quote)
				in_quote = 2;
			else if (in_quote == 2)
				in_quote = 0;
		}
	}
	if (in_quote != 0)
		wait_close(2, in_quote != 2, vars);
	return (in_quote != 0);
}
