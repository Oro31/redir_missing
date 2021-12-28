/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 09:15:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/24 21:27:44 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_pwd(void)
{
	char	*pwd;

	g_exit_status = 0;
	pwd = NULL;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("malloc failed");
		return (2);
	}
	ft_print_str(pwd);
	write(1, "\n", 1);
	free(pwd);
	return (0);
}
