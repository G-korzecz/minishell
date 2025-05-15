/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:12:49 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/15 15:14:24 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	builtin_exit(t_list *cmd, int *is_exit, t_cmd_set *p)
{
	long long int	status[2];
	char			**args;

	args = ((t_cmd *)cmd->content)->args;
	*is_exit = !cmd->next;
	if (*is_exit)
		printf("exit\n");
	if (!args || !args[1])
		free_exit(p, 0, NULL);
	status[1] = ft_atoi(args[1]);
	if (args[1][0] == '\0' || (status[1] < 0 && ft_strlen(args[1]) > 18)
		|| (status[1] == 0 && args[1][0] != '0'))
	{
		*is_exit = 1;
		ft_printf_fd(2, "mini: exit: ");
		ft_printf_fd(2, "%s", args[1]);
		free_exit(p, (2 + status[1]) & 255, ": numeric argument required\n");
	}
	else if (args[2])
	{
		*is_exit = 0;
		put_err(NULL, "exit: too many arguments", 1, 0);
	}
	if (*is_exit)
		free_exit(p, status[1] & 255, NULL);
}
