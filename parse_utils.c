
#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

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
	if (ft_strncmp(before, "", 1) != 0)
		result[0] = ft_strdup(before);
	if (ft_strncmp(after, "", 1) != 0)
		result[2] = ft_strdup(after);
	result[1] = ft_strdup(key);
	return (result);
}

char	*strip(char *str)
{
	char	*end;

	while (is_space((unsigned char)*str))
		str++;
	if (*str == '\0')
		return (str);
	end = str + ft_strlen(str) - 1;
	while (end > str && is_space((unsigned char)*end))
		end--;
	*(end + 1) = '\0';
	return (str);
}

void	append_doubles(char **dest, char **src, int condition)
{
	int	i;
	int	len;
	int	null_check;
	int	max_index;

	if (condition)
		max_index = 3;
	else
		max_index = double_counter(src);
	len = 0;
	i = -1;
	while (++i < max_index)
		if (src[i])
			len += ft_strlen(src[i]);
	i = -1;
	null_check = 0;
	if (condition)
		while (++i < 3)
			if (src[i] == NULL)
				null_check++;
	if (null_check == 3)
	{
		null_free(dest);
		return ;
	}
	null_free(dest);
	*dest = ft_calloc(len + 1, 1);
	i = -1;
	while (++i < max_index)
		if (src[i])
			ft_strlcat(*dest, src[i], len + 1);
}