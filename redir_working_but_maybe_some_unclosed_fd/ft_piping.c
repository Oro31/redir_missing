/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 09:15:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/27 05:46:12 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	ft_exec_pipe(t_env **env, t_cmd **cmd)
{
	char	**path_split;	
	char	**env_p;
	char	*path;
	int		fd[2];

	ft_get_fd(&fd, cmd);
	if (fd[0])
		dup2(fd[0], STDIN_FILENO);
	if (fd[1])
		dup2(fd[1], STDOUT_FILENO);
	if (ft_is_builtin((*cmd)->argv[0]))
	{
		ft_exec_builtin(env, ft_clean_redir(cmd));
		return (0);
	}
	path = ft_get_env(*env, "PATH");
	if (!path)
	{
		return (ft_print_ex_error("No such file or directory",
			(*cmd)->argv[0]));
	}
	path_split = ft_split(&path, ':');
	path = ft_find_valid_path(&path_split, (*cmd)->argv[0]);
	if (path)
	{
		env_p = ft_tenv_to_envp(*env);
		if (execve(path, ft_clean_redir(cmd), env_p) < 0)
		{
			free(path);
			ft_free_ctab(&path_split);
			ft_free_ctab(&env_p);
			perror((*cmd)->argv[0]);
			return (-1);
		}
	}
	free(path);
	ft_free_ctab(&path_split);
	return (ft_print_ex_error("command not found", (*cmd)->argv[0]));
}

int	ft_piping(t_env **env, t_cmd **cmd)
{
	t_cmd				*tmp;
	pid_t				pid;
	struct sigaction	action;
	int					pipe_fd[2];
	int					oldfd;
	int					ret;
	int					status;
	int					i;

	action.sa_handler = ft_pipe_handler;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	ret = 0;
	i = 0;
	tmp = *cmd;
	oldfd = dup(STDIN_FILENO);
	while (tmp)
	{
		pipe(pipe_fd);
		pid = fork();
		if (!pid)
		{
			if (i != 0)
				dup2(oldfd, STDIN_FILENO);
			if (tmp->nxt)
				dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			close(oldfd);
			ret = ft_exec_pipe(env, &tmp);
			exit(ret);
		}
		else if (pid < 0)
			write(2, "error pid\n", 10);
		dup2(pipe_fd[0], oldfd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		tmp = tmp->nxt;
		i++;
	}
	close(oldfd);
	i = -1;
	while (++i < ft_size_list_cmd(*cmd))
		wait(&status);
	return (ret);
}
