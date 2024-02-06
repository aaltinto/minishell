#include "../minishell.h"
#include "../libft/libft.h"

void	echo(t_vars *vars, char *input)
{
	char	**split_commadns;
	int		i;

	split_commadns = ft_split(input, ' ');
	i = 0;
	if (ft_strncmp(split_commadns[1], "-n", 3) == 0)
		i++;
	while (split_commadns[++i])
	{
		ft_putstr_fd(split_commadns[i], 1);
		if (split_commadns[i + 1] != NULL)
			ft_putstr_fd(" ", 1);
	}
	if (ft_strncmp(split_commadns[1], "-n", 3) != 0)
		ft_putstr_fd("\n", 1);
}
