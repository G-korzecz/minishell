/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:30:08 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:45:33 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exec_cmd_and_wait(t_cmd_set *p, int status, int tmp[2], int *is_exit)
{
	tmp[1] = handle_builtins_exit(p, p->cmds, is_exit, 0);
	if (ft_lstsize(p->cmds) == 1)
		p->status_code = tmp[1];
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (p && p->pid_of_lst_cmd != 0)
	{
		waitpid(p->pid_of_lst_cmd, &status, 0);
		p->pid_of_lst_cmd = 0;
	}
	waitpid(-1, NULL, 0);
	if (WIFSIGNALED(status))
		signals_child(WTERMSIG(status));
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 127 && !tmp[1])
			p->status_code = WEXITSTATUS(status);
		else if (!tmp[1])
			p->status_code = WEXITSTATUS(status);
		else if (status && WEXITSTATUS(status))
			p->status_code = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status) && status != 13)
		p->status_code = 128 + WTERMSIG(status);
	p->status_code = p->status_code & 255;
}

static void	*dup_io_fds(t_list *cmd, int fd[2])
{
	t_cmd	*node;

	node = cmd->content;
	if (node->in_fd != 0)
	{
		if (dup2(node->in_fd, 0) == -1)
			return (put_err("DupForkPipe_Failed", NULL, 1, NULL));
		if (node->in_fd != -1)
			close(node->in_fd);
	}
	if (node->out_fd != 1)
	{
		if (dup2(node->out_fd, 1) == -1)
			return (put_err("DupForkPipe_Failed", NULL, 1, NULL));
		if (node->out_fd != -1)
			close(node->out_fd);
	}
	else if (cmd->next && dup2(fd[1], 1) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, NULL));
	if (fd[1] != -1)
		close(fd[1]);
	return (NULL);
}

void	*exec_in_child(t_cmd_set *p, t_list *cmd, int fd[2])
{
	t_cmd	*n;
	int		l;

	n = cmd->content;
	l = 0;
	if (n->args)
		l = ft_strlen(*n->args);
	dup_io_fds(cmd, fd);
	close(fd[0]);
	handle_child_builtins(p, n, l, cmd);
	if (cmd->next != NULL)
		p->status_code = 0;
	l = p->status_code;
	if (p && p->cmds)
		ft_lstclear(&p->cmds, free_lst);
	if (p && p->envp)
		free_array(&p->envp);
	exit(l);
}

void	create_fork(t_cmd_set *p, t_list *cmd, int fd[2])
{
	pid_t	pid;

	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	pid = fork();
	if (!cmd->next && pid > 0)
		p->pid_of_lst_cmd = pid;
	if (pid < 0)
	{
		if (fd[0] != -1)
			close(fd[0]);
		if (fd[1] != -1)
			close(fd[1]);
		put_err("DupForkPipe_Failed", NULL, 1, p);
	}
	else if (!pid)
		exec_in_child(p, cmd, fd);
}

void	*chk_perm_call_child(t_cmd_set *p, t_list *cmd, int fd[2])
{
	t_cmd	*n;
	DIR		*dir;

	n = cmd->content;
	dir = NULL;
	if (n->args)
		dir = opendir(*n->args);
	if (n->in_fd == -1 || n->out_fd == -1)
		return (NULL);
	if ((n->cmd_path && access(n->cmd_path, X_OK) == 0) || is_builtin(n))
		create_fork(p, cmd, fd);
	else if (!is_builtin(n) && ((n->cmd_path && !access(n->cmd_path, F_OK))
			|| dir))
		p->status_code = 126;
	else if (!is_builtin(n) && n->args)
		p->status_code = 127;
	if (dir)
		closedir(dir);
	return ("");
}
