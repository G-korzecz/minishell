/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:37:29 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/17 15:37:31 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_builtin(t_cmd *n)
{
	int	len;

	if (!n->args)
		return (0);
	if ((n->args && ft_strchr(*n->args, '/')) || (n->cmd_path
			&& ft_strchr(n->cmd_path, '/')))
		return (0);
	len = ft_strlen(*n->args);
	if (!ft_strncmp(*n->args, "pwd", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "env", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "cd", len) && len == 2)
		return (1);
	if (!ft_strncmp(*n->args, "export", len) && len == 6)
		return (1);
	if (!ft_strncmp(*n->args, "unset", len) && len == 5)
		return (1);
	if (!ft_strncmp(*n->args, "echo", len) && len == 4)
		return (1);
	if (!ft_strncmp(*n->args, "exit", len) && len == 4)
		return (1);
	return (0);
}

void	*forked_builtins_and_rest(t_cmd_set *p, t_list *cmd)
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

int	handle_builtins_exit(t_cmd_set *p, t_list *cmd, int *is_exit, int n)
{
	char	**a;
	int		s;
	int		one;

	s = 0;
	one = ft_lstsize(p->cmds) == 1;
	while (cmd)
	{
		a = ((t_cmd *)cmd->content)->args;
		n = 0;
		if (a)
			n = ft_strlen(*a);
		if (a && n == 4 && !ft_strncmp(*a, "exit", 4) && one)
			builtin_exit(cmd, is_exit, p);
		else if (a && n == 2 && !ft_strncmp(*a, "cd", 2) && one)
			s = builtin_cd(p, a);
		else if (a && n == 6 && !ft_strncmp(*a, "export", 6) && one)
			s = builtin_export(p, a);
		else if (a && n == 5 && !ft_strncmp(*a, "unset", 5) && one)
			s = builtin_unset(p, a);
		else
			forked_builtins_and_rest(p, cmd);
		cmd = cmd->next;
	}
	return (s);
}

void	handle_child_builtins(t_cmd_set *p, t_cmd *n, int l, t_list *cmd)
{
	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	p->status_code = 0;
	if (!is_builtin(n) && n->args)
		execve(n->cmd_path, n->args, p->envp);
	else if (n->args && !ft_strncmp(*n->args, "pwd", l) && l == 3)
		p->status_code = builtin_pwd();
	else if (n->args && !ft_strncmp(*n->args, "echo", l) && l == 4)
		p->status_code = builtin_echo(cmd);
	else if (n->args && !ft_strncmp(*n->args, "env", l) && l == 3)
		p->status_code = builtin_env(p->envp);
	else if (n->args && !ft_strncmp(*n->args, "exit", l) && l == 4)
		p->status_code = builtin_exit_child(cmd);
	else if (n->args && !ft_strncmp(*n->args, "cd", l) && l == 2)
		p->status_code = builtin_cd(p, n->args);
	else if (n->args && !ft_strncmp(*n->args, "export", l) && l == 6)
		p->status_code = builtin_export(p, n->args);
	else if (n->args && !ft_strncmp(*n->args, "unset", l) && l == 5)
		p->status_code = builtin_unset(p, n->args);
	if (cmd->next)
		p->status_code = 0;
}
