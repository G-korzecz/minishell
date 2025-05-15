/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parent_builtin.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 02:26:41 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/15 02:30:31 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_parent_builtins(t_cmd_set *p, t_list *cmd, int *is_exit, int n)
{
	int		list_size;
	char	**a;
	int		status_code;

	list_size = ft_lstsize(p->cmds);
	while (cmd)
	{
		status_code = 0;
		a = ((t_cmd *)cmd->content)->args;
		n = 0;
		if (a)
			n = ft_strlen(*a);
		if (a && !ft_strncmp(*a, "exit", n) && n == 4 && list_size == 1)
			builtin_exit(cmd, is_exit, p);
		else if (list_size == 1 && !cmd->next && a && !ft_strncmp(*a, "cd", n) && n == 2)
			status_code = builtin_cd();
		else if (list_size == 1 && !cmd->next && a && !ft_strncmp(*a, "export", n) && n == 6)
			status_code = builtin_export();
		else if (list_size == 1 && !cmd->next && a && !ft_strncmp(*a, "unset", n) && n == 5)
			status_code = builtin_unset();
		else
			setup_command_pipe(p, cmd);
		if (cmd->next)
			status_code = 0;
		cmd = cmd->next;
	}
	return (status_code);
}
