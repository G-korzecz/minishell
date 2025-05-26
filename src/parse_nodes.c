/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:37:49 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:37:55 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Processes one token at index i, checking for redirections or arguments.
Handles append/truncate/redirect/pipe tokens and errors.
Updates the command list or prints an error if invalid. */
static t_cmd	*check_redir_pipe(t_cmd *t, char **a[2], int *i, t_cmd_set *p)
{
	char	*err;

	err = is_invalid_syntax(a[0], i);
	if (err && ft_strncmp(err, "eof", 3))
		return (*i = -1, put_err("Unexpected_Token", err, 2, p), t);
	else if (err)
		return (*i = -1, put_err_syntax(err, 2, p), t);
	if (a[0][*i][0] == '>' && a[0][*i + 1]
		&& a[0][*i + 1][0] == '>' && g_exit_status != 130)
		t = out_fd_append(t, a[1], i, p);
	else if (a[0][*i][0] == '>' && g_exit_status != 130)
		t = out_fd_truncate(t, a[1], i, p);
	else if (a[0][*i][0] == '<' && a[0][*i + 1]
		&& a[0][*i + 1][0] == '<')
		t = in_fd_heredoc(t, a[1], i, p);
	else if (a[0][*i][0] == '<')
		t = in_fd_read(t, a[1], i, p);
	else if (a[0][*i][0] != '|')
		t->args = ft_array_insert(t->args, a[1][*i]);
	return (t);
}

static char	**args_after_quotes_removed(char **args)
{
	char	**temp;
	char	*tmpstr;
	int		j;

	j = -1;
	temp = ft_dup_array(args);
	while (temp && temp[++j])
	{
		tmpstr = remove_quotes(temp[j], 0, 0);
		free(temp[j]);
		temp[j] = tmpstr;
	}
	return (temp);
}

static t_list	*parse_cmds(char **args, int i, t_cmd_set *p)
{
	t_list	*cmds[2];
	char	**temp[2];

	cmds[0] = NULL;
	if (args && args[0] && args[0][0] == '|')
		return (put_err("Unexpected_Token", "|", 2, p),
			free_tmp_lst(NULL, args, NULL));
	temp[1] = args_after_quotes_removed(args);
	while (args[++i])
	{
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
		{
			i += args[i][0] == '|';
			ft_lstadd_back(&cmds[0], ft_lstnew(init_cmd()));
		}
		cmds[1] = ft_lstlast(cmds[0]);
		temp[0] = args;
		cmds[1]->content = check_redir_pipe(cmds[1]->content, temp, &i, p);
		if (i < 0)
			return (free_tmp_lst(cmds[0], args, temp[1]));
	}
	return (free_array(&temp[1]), free_array(&args), cmds[0]);
}

/* Parses and executes the given arguments.
Uses parse_cmds to build the command list.
Sets the $_ variable and calls exec_cmd_and_wait.
Returns the original pointer after execution. */
void	*parse_nodes(char **args, t_cmd_set *p)
{
	int	is_exit;
	int	status;
	int	tmp[2];

	is_exit = 0;
	status = 0;
	tmp[0] = -1;
	tmp[1] = -1;
	p->cmds = parse_cmds(split_with_special_chars(args), -1, p);
	if (!p->cmds)
		return (p);
	if (ft_lstsize(p->cmds) == 1 && ((t_cmd *)(p->cmds->content))->args
		&& ft_arr_len(((t_cmd *)(p->cmds->content))->args))
		ft_setenv("_", ((t_cmd *)(p->cmds->content))->args[ft_arr_len(((t_cmd *)
					(p->cmds->content))->args) - 1], p->envp);
	exec_cmd_and_wait(p, status, tmp, &is_exit);
	if (is_exit)
		free_exit(p, p->status_code, NULL);
	if (p->cmds && is_exit)
	{
		ft_lstclear(&p->cmds, free_lst);
		return (NULL);
	}
	return (p);
}
