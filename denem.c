#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minishell.h"
#include "libft/libft.h"

char	**split_string(char *src, char *key)
{
	char	**result;
	char	*before;
	char	*after;
	char	*found;	

	result = malloc(3 * sizeof(char *));
	if (result == NULL)
		return (err_msg("Memory allocation failed\n", 1), NULL);
	found = ft_strnstr(src, key, ft_strlen(src));
	if (found != NULL)
	{
		before = src;
		*found = '\0';
		after = found + strlen(key);
	}
	result[0] = ft_strdup(before);
	result[1] = ft_strdup(key);
	result[2] = ft_strdup(after);
	return (result);
}

int main() {
    char src[] = "sample string";
    char key[] = "sample";

    printf("Source: %s\n", src);
    printf("Key to search: %s\n", key);

    char **result = split_string(src, key);

    printf("Before: %s\n", result[0] != NULL ? result[0] : "Not found");
    printf("Key: %s\n", result[1] != NULL ? result[1] : "Not found");
    printf("After: %s\n", result[2] != NULL ? result[2] : "Not found");
    for (int i = 0; i < 3; i++)
	{
        free(result[i]);
    }
    free(result);

    return 0;
}
