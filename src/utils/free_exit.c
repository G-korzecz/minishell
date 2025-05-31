/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwang <cwang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:28 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/28 16:25:23 by cwang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Exit and cleaning function. Can print something if needed.
Free everything that is allocated in the main struct :
- p->envp as an **array
- p->cmd as a command list (see lst_clear)
- p->input_text as a *char.*/
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
