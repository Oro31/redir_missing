/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 12:33:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/23 00:41:04 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/*this set of function put space around valid redirections
this will be usefull later when we will handle redirections
everything that is not return is freed
the function ft_set_redir allocate memory with malloc
you need to free it before exiting your program*/

static int	ft_is_sepredir(char *line, int i)
{
	if (i == 0)
		return (0);
	if (ft_is_char_quoted(line, i, 0, line[i]))
		return (0);
	if (line[i] != ' ' && !ft_is_redir(line[i]) && ft_is_redir(line[i - 1]))
		return (1);
	if (line[i - 1] != ' ' && ft_is_redir(line[i])
		&& !ft_is_redir(line[i - 1]))
		return (1);
	return (0);
}

static void	ft_fill_set_redir(char **tmp, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_is_sepredir(str, i))
		{
			(*tmp)[j] = ' ';
			(*tmp)[j + 1] = str[i];
			j += 2;
		}
		else
		{
			(*tmp)[j] = str[i];
			j++;
		}
		i++;
	}
}

static char	*ft_put_space(char *str)
{
	char	*tmp;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_is_sepredir(str, i))
			j += 2;
		else
			j++;
		i++;
	}
	if (!ft_malloc_splchar(&tmp, j + 1))
		return (NULL);
	ft_fill_set_redir(&tmp, str);
	return (tmp);
}

char	**ft_set_redir(char **cmd_char)
{
	char	**tmp;
	int		i;

	i = 0;
	while (cmd_char[i])
		i++;
	if (!ft_malloc_dblchar(&tmp, i + 1))
		return (NULL);
	i = -1;
	while (cmd_char[++i])
	{
		tmp[i] = ft_put_space(cmd_char[i]);
		free(cmd_char[i]);
	}
	free(cmd_char);
	return (tmp);
}
