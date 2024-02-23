
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

	i = -1;
	while (str[++i])
	{
		if (!str[i])
			free(str[i]);
	}
	if (!str)
		free(str);
}
