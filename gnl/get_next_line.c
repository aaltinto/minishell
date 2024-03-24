/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:39:05 by aaltinto          #+#    #+#             */
/*   Updated: 2023/08/02 13:39:09 by aaltinto         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*trim_str(char *str)
{
	char	*res;
	size_t	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] != '\n')
		i++;
	if (str[i + 1] == '\0')
		return (free_str(&str, 0));
	res = ft_substr2(str, i + 1, ft_strlen2(str));
	if (!res)
	{
		free_str(&str, 0);
		return (NULL);
	}
	free_str(&str, 0);
	return (res);
}

static char	*get_line(char *str)
{
	char	*line;
	size_t	i;
	size_t	j;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	j = 0;
	while (j < i + 1)
	{
		line[j] = str[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*integrate_str(char *str, char *buf)
{
	char	*res;

	if (!str && buf)
	{
		res = ft_strdup2(buf);
		if (!res)
			return (NULL);
		return (res);
	}
	res = ft_strjoin2(str, buf);
	free_str(&str, 0);
	return (res);
}

static int	nl_check(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i] != '\0')
		if (str[i] == '\n')
			return (1);
	return (0);
}

char	*get_next_line(int fd)
{
	char		buf[BUFFER_SIZE + 1];
	ssize_t		rd;
	static char	*str;
	char		*line;

	rd = 1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_str(&str, 0));
	while (rd > 0)
	{
		rd = read(fd, buf, BUFFER_SIZE);
		if ((rd <= 0 && !str) || rd == -1)
			return (free_str(&str, 0));
		buf[rd] = '\0';
		str = integrate_str(str, buf);
		if (nl_check(str))
		{
			line = get_line(str);
			if (!line)
				return (free_str(&str, 0));
			return (str = trim_str(str), line);
		}
	}
	return (free_str(&str, 1));
}
