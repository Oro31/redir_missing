/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 12:33:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/26 06:35:58 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	ft_main_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
}

void	ft_fork_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_status = 130;
	}
	else if (signum == SIGQUIT)
	{
		write(2, "Quit (core dumped)\n", 19);
		g_exit_status = 131;
	}	
}

void	ft_pipe_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_status = 130;
	}
	else if (signum == SIGQUIT)
	{
		write(2, "Quit (core dumped)", 18);
		g_exit_status = 131;
	}	
}
