
#include "minishell.h"

char	**split_string(char *src, char *key)
{
	char	**result;
	char	*before;
	char	*after;
	char	*found;
	int		i;

	result = ft_calloc(3, sizeof(char *));
	if (result == NULL)
		return (err_msg("Memory allocation failed\n", 1), NULL);
	found = ft_strnstr(src, key, ft_strlen(src));
	if (found != NULL)
	{
		before = src;
		*found = '\0';
		after = found + ft_strlen(key);
	}
	i = -1;
	while (++i < 3)
		result[i] = NULL;
	if (before)
		result[0] = ft_strdup(before);
	if (after)
		result[2] = ft_strdup(after);
	result[1] = ft_strdup(key);
	return (result);
}

void	append_doubles(char **dest, char **src)
{
	int	i;
	int	len;

	len = 0;
	i = -1;
	while (src[++i])
		if (src[i])
			len += ft_strlen(src[i]);
	if (dest)
		free(*dest);
	*dest = ft_calloc(len + 1, 1);
	i = -1;
	while (++i <= double_counter(src))
		if (src[i])
			ft_strlcat(*dest, src[i], len + 1);
}
