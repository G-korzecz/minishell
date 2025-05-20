/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:57:40 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/13 23:58:13 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_echo(t_list *cmd)
{
	int		print_newline;
	int		i;
	int		j;
	char	**args;

	i = 0;
	j = 0;
	print_newline = 1;
	args = ((t_cmd *)cmd->content)->args;
	while (args && args[++i])
	{
		if (!j && !ft_strncmp(args[i], "-n", 2) && (ft_countc(args[i],
					'n') == (int)(ft_strlen(args[i]) - 1)))
			print_newline = 0;
		else
		{
			j = 1;
			ft_printf_fd(1, "%s", args[i]);
			if (args[i + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	if (print_newline)
		write(1, "\n", 1);
	return (0);
}
