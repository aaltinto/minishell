/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:25 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/19 18:46:40 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

void	*new_pwd(void)
{
	char	*pwd;
	
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("getcwd"), NULL);
	ft_putendl_fd(pwd, 1);
	return (null_free(&pwd), NULL);
}
