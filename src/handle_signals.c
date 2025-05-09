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

int		g_exit_status;

/* This function is called from main inside while loop.
calls signals_parent to set parent signals
SIGQUIT (ctlr + \ ) will be ignored (SIG_IGN)
checks if there was a signal status code before,
then set p->status_code to this signal status code */
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

/* Sets signals for the program (parent process). */
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

/* Sets signals for the child process. */
void	signals_child(int signal_code)
{
	if (signal_code == SIGINT)
	{
		ft_printf_fd(1, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_exit_status = 130;
	}
	else if (signal_code == SIGQUIT)
	{
		ft_printf_fd(2, "Quit (core dumped)\n");
		rl_replace_line("", 0);
		g_exit_status = 131;
		rl_on_new_line();
	}
}

/* Sets signals for the heredoc. */
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
