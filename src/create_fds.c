/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:29:05 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:29:26 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*Opens a file at "path" with appropriate flags.
Closes "oldfd" if it's valid (> 2).
Returns the new file descriptor or -1 on failure and sets error status.*/
int	get_fd(int oldfd, char *path, int flags[2], t_cmd_set *p)
{
	int	fd;

	fd = -1;
	if (oldfd > 2)
		close(oldfd);
	if (!path)
		return (-1);
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0664);
	else if (flags[0])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0664);
	else
		fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			put_err("Perm_Denied", path, 126, p);
		else if (errno == ENOENT)
			put_err("NoFile_NoDir", path, 127, p);
		else
			put_err("Open_Failed", path, 1, p);
	}
	return (fd);
}

/* Sets the out_fd in truncate mode i.e. ls > out.txt
Calls the get_fd to create the file.
Truncate will delete the old content of file and add new content */
t_cmd	*out_fd_truncate(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		node->out_fd = get_fd(node->out_fd, args[*i], flags, p);
	if (!args[*i] || node->out_fd == -1)
	{
		*i = -1;
		if (node->out_fd != -1)
		{
			error_token_newline();
			p->status_code = 2;
		}
		else
			p->status_code = 1;
	}
	return (node);
}

/* Sets the out_fd in append mode i.e. ls >> out.txt
Calls the get_fd to create the file. */
t_cmd	*out_fd_append(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 1;
	(*i)++;
	if (args[++(*i)])
		node->out_fd = get_fd(node->out_fd, args[*i], flags, p);
	if (!args[*i] || node->out_fd == -1)
	{
		*i = -1;
		if (node->out_fd != -1)
		{
			error_token_newline();
			p->status_code = 2;
		}
		else
			p->status_code = 1;
	}
	return (node);
}

/* Sets in_fd to read from a file i.e.  <input.txt cat
Calls the get_fd to create the file.
Append will not delete old content, but add text to the end of file. */
t_cmd	*in_fd_read(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 0;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		node->in_fd = get_fd(node->in_fd, args[*i], flags, p);
	if (!args[*i] || node->in_fd == -1)
	{
		if (node->in_fd != -1)
		{
			error_token_newline();
			p->status_code = 2;
		}
		if (p && p->cmds && ft_lstsize(p->cmds) != 1)
			p->status_code = 0;
		else
			(*i)--;
	}
	return (node);
}
