/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 14:05:31 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/26 08:06:42 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

/*this function is used to add a new element at the top
of a list of type t_cmd, it returns the pointer to this new element*/

t_cmd	*ft_hadd_tolist_cmd(t_cmd *list, char **split, int is_pipe)
{
	t_cmd	*newelem;

	newelem = malloc(sizeof(t_cmd));
	if (!newelem)
	{
		perror("malloc failed");
		return (NULL);
	}
	newelem->argv = split;
	newelem->is_cmd_after_pipe = is_pipe;
	newelem->nxt = list;
	return (newelem);
}

/*this function is used to add a new element at the end
of a list of type t_cmd, it returns the pointer to the head of the list*/

t_cmd	*ft_eadd_tolist_cmd(t_cmd *list, char **split, int is_pipe)
{
	t_cmd	*newelem;
	t_cmd	*tmp;

	newelem = malloc(sizeof(t_cmd));
	if (!newelem)
	{
		perror("malloc failed\n");
		return (NULL);
	}
	newelem->argv = split;
	newelem->is_cmd_after_pipe = is_pipe;
	newelem->nxt = NULL;
	if (list == NULL)
		return (newelem);
	else
	{
		tmp = list;
		while (tmp->nxt != NULL)
			tmp = tmp->nxt;
		tmp->nxt = newelem;
		return (list);
	}
}

/*this function is used to free all elements of a listof type t_cmd
everything that were allocated in each element are freed*/

void	ft_free_list_cmd(t_cmd **list)
{
	t_cmd	*tmp;
	int		i;

	if (list)
	{
		if (*list)
		{
			while (*list)
			{
				tmp = (*list)->nxt;
				i = -1;
				if ((*list)->argv)
					while ((*list)->argv[++i])
						free((*list)->argv[i]);
				free((*list)->argv);
				(*list)->argv = NULL;
				free(*list);
				*list = NULL;
				*list = tmp;
			}
		}
	}
}
