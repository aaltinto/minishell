#ifndef MINISHELL_H
#define MINISHELL_H

typedef struct s_vars
{
	char	*path;
	char	**command;
	char	**env;
}	t_vars;

void	err_msg(char *msg, int x);
void	free_doubles(char **str);

#endif