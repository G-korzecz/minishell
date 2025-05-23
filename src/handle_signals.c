/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:33:23 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:33:27 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

volatile sig_atomic_t	g_exit_status;

void	set_signals(t_cmd_set *p)
{
	signal(SIGINT, signals_parent);
	signal(SIGQUIT, SIG_IGN);
	if (p->status_code < 0)
		p->status_code = 0;
	if (g_exit_status >= 0)
	{
		p->status_code = g_exit_status;
		g_exit_status = -1;
	}
}

void	signals_parent(int signal_code)
{
	if (signal_code == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_status = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signals_child(int signal_code)
{
	if (signal_code == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_exit_status = 130;
	}
	else if (signal_code == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 19);
		rl_replace_line("", 0);
		g_exit_status = 131;
		rl_on_new_line();
	}
}

void	signals_heredoc(int signal_code)
{
	if (signal_code == SIGINT)
	{
		g_exit_status = 130;
		ioctl(0, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	disable_ctrl_z(void)
{
	signal(SIGTSTP, SIG_IGN);
}
