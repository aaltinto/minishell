#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

void	err_msg(char *msg, int x)
{
	if (x == 1)
		ft_putendl_fd(msg, 2);
	if (x == 0)
		ft_putstr_fd(msg, 2);
}

void	free_doubles(char **str)
{
	int	i;

	if (!str)
		return ;
	i = -1;
	while (str[++i])
		null_free(&str[i]);
	free(str);
	str = NULL;
}
void	free_doubles2(void **str, int j)
{
	int	i;

	if (!str)
		return ;
	i = -1;
	while (j > ++i)
	{
		printf("str[%d] freed\n", i);
		null_free_void(&str[i]);
	}
	free(str);
	str = NULL;
}

void	null_free(char **var)
{
	if (!*var)
		return ;
	free(*var);
	*var = NULL;
}
void	null_free_void(void **var)
{
	if (!*var)
		return ;
	free(*var);
	*var = NULL;
}
