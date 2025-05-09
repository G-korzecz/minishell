/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:13:09 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/29 17:13:26 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Sets up a pipe for the given command and handles execution.
Calls find_cmd_path to resolve the binary path.
Creates a pipe and calls chk_perm_call_child to fork and exec if needed.
Closes write-end of the pipe after fork.
Passes the read-end to the next command if it exists.
Closes input/output fds of the current command if > 2. */
void	*setup_command_pipe(t_cmd_set *p, t_list *cmd)
{
	int	fd[2];

	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	find_cmd_path(p, cmd, NULL, NULL);
	if (pipe(fd) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p));
	if (!chk_perm_call_child(p, cmd, fd))
		return (NULL);
	close(fd[1]);
	if (cmd->next && !((t_cmd *)cmd->next->content)->in_fd)
		((t_cmd *)cmd->next->content)->in_fd = fd[0];
	else
		close(fd[0]);
	if (((t_cmd *)cmd->content)->in_fd > 2)
		close(((t_cmd *)cmd->content)->in_fd);
	if (((t_cmd *)cmd->content)->out_fd > 2)
		close(((t_cmd *)cmd->content)->out_fd);
	return (NULL);
}

/* Executes the command using execve in the child process.
Sets child signal handlers before execution.
Exits with:
- 127 if command not found or ENOENT,
- 126 if permission denied or it's a directory,
- 1 for all other errors. */
void	run_execve(t_cmd_set *p, t_cmd *n)
{
	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	if (!n->cmd_path)
		exit(127);
	execve(n->cmd_path, n->args, p->envp);
	if (errno == ENOENT)
		exit(127);
	else if (errno == EACCES || errno == EISDIR)
		exit(126);
	else
		exit(1);
}
