/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 09:15:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/26 08:32:49 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*ft_set_path(char *path, char *value)
{
	char	*ret;
	int		i;

	ret = NULL;
	i = -1;
	while (path[++i])
		ret = ft_realloc(&ret, path[i]);
	ret = ft_realloc(&ret, '/');
	i = -1;
	while (value[++i])
		ret = ft_realloc(&ret, value[i]);
	return (ret);
}

int	ft_print_ex_error(char *error, char *cmd)
{
	g_exit_status = 127;
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	return (127);
}

char	*ft_find_valid_path(char ***path_split, char *cmd)
{
	char	*path;
	int		i;

	path = NULL;
	i = -1;
	while ((*path_split)[++i])
	{
		path = ft_set_path((*path_split)[i], cmd);
		if (!access(path, X_OK))
		{
			i = -1;
			while ((*path_split)[++i])
			{
				free((*path_split)[i]);
				(*path_split)[i] = NULL;
			}
			free(*path_split);
			*path_split = NULL;
			return (path);
		}
		free(path);
		path = NULL;
	}
	return (NULL);
}

int	ft_execve(char **argv, t_env **env)
{
	char	**path_split;
	char	**env_p;
	char	*path;

	path = ft_get_env(*env, "PATH");
	if (!path)
		return (ft_print_ex_error("No such file or directory", argv[0]));
	path_split = ft_split(&path, ':');
	path = ft_find_valid_path(&path_split, argv[0]);
	if (path)
	{
		env_p = ft_tenv_to_envp(*env);
		if (execve(path, argv, env_p) < 0)
		{
			free(path);
			ft_free_ctab(&path_split);
			ft_free_ctab(&env_p);
			perror(argv[0]);
			return (-1);
		}
	}
	free(path);
	ft_free_ctab(&path_split);
	return (ft_print_ex_error("command not found", argv[0]));
}

int	ft_hdl_arg_execve(t_env **env, char **argv)
{
	pid_t				pid;
	struct sigaction	action;
	int					ret;
	int					status;

	action.sa_handler = ft_fork_handler;
	ret = 0;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	pid = fork();
	if (!pid)
	{
		ret = ft_execve(argv, env);
		exit(ret);
	}
	else if (pid < 0)
		write(2, "error pid\n", 10);
	wait(&status);
	return (ret);
}
