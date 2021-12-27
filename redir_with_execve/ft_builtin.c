/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 12:33:23 by rvalton           #+#    #+#             */
/*   Updated: 2021/12/26 07:55:29 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_is_builtin(char *exe)
{
	if (ft_strcomp("env", exe) == 0)
		return (1);
	else if (ft_strcomp("cd", exe) == 0)
		return (1);
	else if (ft_strcomp("unset", exe) == 0)
		return (1);
	else if (ft_strcomp("export", exe) == 0)
		return (1);
	else if (ft_strcomp("pwd", exe) == 0)
		return (1);
	else if (ft_strcomp("echo", exe) == 0)
		return (1);
	else if (ft_strcomp("exit", exe) == 0)
		return (1);
	return (0);
}

int	ft_exec_builtin(t_env **env, char **argv)
{
	if (ft_strcomp("env", argv[0]) == 0)
		return (ft_env(*env, argv));
	else if (ft_strcomp("cd", argv[0]) == 0)
		return (ft_hdl_arg_cd(env, argv));
	else if (ft_strcomp("unset", argv[0]) == 0)
		return (ft_hdl_arg_unset(env, argv));
	else if (ft_strcomp("export", argv[0]) == 0)
		return (ft_hdl_arg_export(env, argv));
	else if (ft_strcomp("pwd", argv[0]) == 0)
		return (ft_pwd());
	else if (ft_strcomp("echo", argv[0]) == 0)
		return (ft_echo(argv));
	else if (ft_strcomp("exit", argv[0]) == 0)
		return (ft_exit(NULL, NULL, NULL, argv));
	else
		return (2);
}
