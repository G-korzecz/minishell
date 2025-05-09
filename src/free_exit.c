/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:28 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/22 19:24:08 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Frees all allocated memory and exits the shell.
Prints the message to stderr if provided.
Cleans up input, environment, command list, and readline history. */
void	free_exit(t_cmd_set *p, int exit_code, char *msg)
{
	if (msg)
		ft_printf_fd(2, msg);
	if (p && p->input_text != NULL)
		free(p->input_text);
	if (p && p->envp && *p->envp)
		free_array(&p->envp);
	if (p && p->cmds)
		ft_lstclear(&p->cmds, free_lst);
	rl_clear_history();
	exit(exit_code);
}
