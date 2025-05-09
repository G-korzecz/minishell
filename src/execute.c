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

/*If the cmd was internal (cd,unset,export,exit) runs builtin function.
If there are child processes waits for all to finish.
Then gets status from the child process and sets the p->status_code. */
void	exec_cmd_and_wait(t_cmd_set *p, int status)
{
	t_list	*cmd;

	cmd = p->cmds;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (cmd)
	{
		setup_command_pipe(p, cmd);
		cmd = cmd->next;
	}
	if (p && p->pid_of_lst_cmd != 0)
	{
		waitpid(p->pid_of_lst_cmd, &status, 0);
		p->pid_of_lst_cmd = 0;
	}
	waitpid(-1, NULL, 0);
	if (WIFSIGNALED(status))
		signals_child(WTERMSIG(status));
	if (WIFEXITED(status))
		p->status_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status) && status != 13)
		p->status_code = 128 + WTERMSIG(status);
	p->status_code = p->status_code & 255;
}

/* Duplicates file descriptors, if in_fd is specified -> to stdin.
If out_fd is specified -> to stdout .
After duplicate, closes the oldfds. */
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

/* Calls function to set redirections.
Runs external cmd or internal (pwd,echo,env) in child process. */
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
	run_execve(p, n);
	if (cmd->next != NULL)
		p->status_code = 0;
	l = p->status_code;
	if (p && p->cmds)
		ft_lstclear(&p->cmds, free_lst);
	if (p && p->envp)
		free_array(&p->envp);
	exit(l);
}

/* Creates child process, call func to run the cmd in child process. */
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

/* Checks if the cmd is executable, if not checks if it is a directory.
If it is a directory sets status_code to 126.
If it is not a directory sets status_code to 127.
If it's executable then calls the create_fork function. */
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
	if (n->cmd_path && access(n->cmd_path, X_OK) == 0)
		create_fork(p, cmd, fd);
	else if ((n->cmd_path && access(n->cmd_path, F_OK) == 0) || dir)
		p->status_code = 126;
	else if (n->args)
		p->status_code = 127;
	if (dir)
		closedir(dir);
	return ("");
}
