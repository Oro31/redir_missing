/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 09:15:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/27 04:01:12 by rvalton          ###   ########.fr       */
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

void	ft_get_fd(int (*fd)[2], t_cmd **cmd)
{
	int	i;
	int	in;
	int	out;

	ft_init_get_fd(fd, &i, &in, &out);
	while ((*cmd)->argv[++i])
	{
		if ((*cmd)->redir[i] <= 2 && (*cmd)->redir[i] > 0)
		{
			if (in)
				close((*fd)[0]);
			(*fd)[0] = open((*cmd)->argv[i + 1], O_RDONLY);
			if ((*fd)[0] == -1)
				exit(1);
			in = 1;
		}
		else if ((*cmd)->redir[i] > 2)
		{
			if (out)
				close((*fd)[1]);
			(*fd)[1] = open((*cmd)->argv[i + 1], O_RDWR | O_CREAT, S_IRWXU);
			if ((*fd)[1] == -1)
				exit(1);
			out = 1;
		}
	}
}
