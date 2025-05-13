/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:35:48 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/13 21:36:18 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_builtin(t_cmd *n)
{
	int	len;

	if (!n->args)
		return (0);
	if ((n->args && ft_strchr(*n->args, '/')) || (n->cmd_path
			&& ft_strchr(n->cmd_path, '/')))
		return (0);
	len = ft_strlen(*n->args);
	if (!ft_strncmp(*n->args, "pwd", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "env", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "echo", len) && len == 4)
		return (1);
	return (0);
}

int	exec_builtin_child(t_cmd_set *p, t_cmd *n, t_list *cmd)
{
	char	*arg;
	int		len;

	if (!n || !n->args || !*n->args)
		return (0);
	arg = *n->args;
	len = ft_strlen(arg);
	if (!ft_strncmp(arg, "pwd", len) && len == 3)
		return (builtin_pwd());
	else if (!ft_strncmp(arg, "echo", len) && len == 4)
		return (builtin_echo(cmd));
	else if (!ft_strncmp(arg, "env", len) && len == 3)
		return (builtin_env(p->envp));
	return (0);
}
