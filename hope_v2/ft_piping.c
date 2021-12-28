/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_piping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 09:15:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/28 06:33:32 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	ft_dup2_close_fd(int i, int oldfd, int pipe_fd[2], t_cmd *tmp)
{
	if (i != 0)
		dup2(oldfd, STDIN_FILENO);
	if (tmp->nxt)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(oldfd);
}

static t_cmd	*ft_nxt_pipe(int *i, int *oldfd, int pipe_fd[2], t_cmd *tmp)
{
	dup2(pipe_fd[0], (*oldfd));
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	(*i)++;
	return (tmp->nxt);
}

static int	ft_exec_pipe(t_env **env, t_cmd **cmd)
{
	char	**path_split;	
	char	**env_p;
	char	*path;
	int		fd[2];

	if (ft_is_builtin((*cmd)->argv[0]))
	{
		ft_exec_builtin(env, cmd);
		return (0);
	}
	if (ft_get_fd(&fd, cmd))
		return (1);
	ft_dup2_fd(fd);
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
	int					status;
	int					i;

	action.sa_handler = ft_pipe_handler;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	i = 0;
	tmp = *cmd;
	oldfd = dup(STDIN_FILENO);
	while (tmp)
	{
		pipe(pipe_fd);
		pid = fork();
		if (!pid)
		{
			ft_dup2_close_fd(i, oldfd, pipe_fd, tmp);
			ft_exec_pipe(env, &tmp);
			exit(g_exit_status);
		}
		else if (pid < 0)
			write(2, "error pid\n", 10);
		tmp = ft_nxt_pipe(&i, &oldfd, pipe_fd, tmp);
	}
	close(oldfd);
	i = -1;
	while (++i < ft_size_list_cmd(*cmd))
		wait(&status);
	return (0);
}
