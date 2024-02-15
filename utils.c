
#include "libft/libft.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	double_counter(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		;
	return (i);
}

int	find_in_env(char **env, char *to_find)
{
	int	i;

	i = -1;
	while (env[++i])
		if (ft_strncmp(env[i], to_find, ft_strlen(to_find)) == 0)
			return (i);
	return (-1);
}
