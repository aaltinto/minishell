
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

void	arg_counter(t_vars *vars)
{
	int		i;
	int		count;
	int		check;
	char	quote_type;

	quote_type = '\0';
	count = 0;
	check = 0;
	i = -1;
	while (vars->input[++i] != '\0')
	{
		if (is_space(vars->input[i]))
			check = 0;
		if (vars->input[i] == '\'' || vars->input[i] == '\"')
		{
			quote_type = vars->input[i];
			check = 0;
			while (vars->input[++i] != quote_type)
				;
			continue ;
		}
		if (check == 0 && ++check)
			count++;
	}
	vars->argc = count;
}

int	parse_quote(int i, t_vars *vars)
{
	int		j;
	char	quote_type;

	j = 0;
	quote_type = vars->input[i];
	if (is_quote(vars->input[i]))
	{
		while (vars->input[++i] != quote_type)
		{
			j++;
		}
	}
	return (j);
}

int	parse(t_vars *vars, int count)
{
	int		i;
	int		len;
	int		in_quotes;
	int		j;

	len = ft_strlen(vars->input);
	vars->input_parsed = (char **)malloc(len * sizeof(char *));
	i = -1;
	in_quotes = 0;
	j = 0;
	while (++i <= len)
	{
		if (vars->input[i] == '"')
			in_quotes = !in_quotes;
		else if ((is_space(vars->input[i]) && !in_quotes) || !vars->input[i])
		{
			if (i > j)
				vars->input_parsed[count++] = ft_substr(vars->input, j, i - j);
			j = i + 1;
		}
	}
	return (free(vars->input), 1);
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
