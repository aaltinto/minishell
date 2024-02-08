
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
		new_input = readline(">");
		i = -1;
		while (new_input[++i])
		{
			if (type && new_input[i] == '\"')
				quote_check++;
			if (!type && new_input[i] == '\'')
				quote_check++;
		}
		tmp = ft_strjoin(vars->input, "\n");
		free(vars->input);
		vars->input = ft_strjoin(tmp, new_input);
		free(tmp);
		free(new_input);
	}
	if (!quote(vars))
		return ;
}

void	quote_vanish(t_vars *vars)
{
	char	*tmp;
	int		in_quote;
	int		i;
	int		j;

	tmp = malloc(ft_strlen(vars->input) + 1);
	if (!tmp)
		return (err_msg("Malloc Error", 1));
	in_quote = 0;
	i = -1;
	j = 0;
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
		else
		    tmp[j++] = vars->input[i];
	}
	tmp[j] = '\0';
	free(vars->input);
	vars->input = ft_strdup(tmp);
	free(tmp);
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
