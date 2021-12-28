/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 09:15:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/28 05:21:00 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	ft_init_get_fd(int (*fd)[2], int *i, int *in, int *out)
{
	(*fd)[0] = 0;
	(*fd)[1] = 1;
	*i = -1;
	*in = 0;
	*out = 0;
}

void	ft_close_fd(int fd[2])
{
	if (fd[0])
		close(fd[0]);
	if (fd[1] > 1)
		close(fd[1]);
}

void	ft_dup2_fd(int fd[2])
{
	if (fd[0])
		dup2(fd[0], STDIN_FILENO);
	if (fd[1] > 1)
		dup2(fd[1], STDOUT_FILENO);
}

char	**ft_clean_redir(t_cmd **cmd)
{
	char	**ret;
	char	*str;
	int		i;

	ret = NULL;
	str = NULL;
	i = -1;
	while ((*cmd)->argv[++i])
	{
		if ((*cmd)->redir[i] > 0)
			i++;
		else
		{
			str = ft_str_copy_nofree((*cmd)->argv[i]);
			ret = ft_double_realloc(&str, &ret);
		}
	}
	return (ret);
}

int	ft_get_fd(int (*fd)[2], t_cmd **cmd)
{
	int	i;
	int	in;
	int	out;

	ft_init_get_fd(fd, &i, &in, &out);
	while ((*cmd)->argv[++i])
	{
		if ((*cmd)->redir[i] <= 2 && (*cmd)->redir[i] > 0)
		{
			if (ft_open_splr(&in, fd, (*cmd)->argv[i + 1]))
				return (1);
		}
		else if ((*cmd)->redir[i] == 3)
		{
			if (ft_open_splw(&out, fd, (*cmd)->argv[i + 1]))
				return (1);
		}
		else if ((*cmd)->redir[i] == 4)
		{
			if (ft_open_dblw(&out, fd, (*cmd)->argv[i + 1]))
				return (1);
		}
	}
	return (0);
}
