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

/* Checks for invalid syntax patterns in the token array.
Handles cases like redirection without argument, or invalid sequences.
Returns a string representing the unexpected token or "newline". */
static char	*is_invalid_syntax(char *t[], int *i)
{
	if (!t[*i])
		return ("newline");
	if ((t[*i][0] == '<' || t[*i][0] == '>') && (!t[*i + 1]))
		return ("newline");
	if (t[*i][0] == '<')
	{
		if (t[*i + 1] && t[*i + 1][0] == '|')
			return ("|");
		if (t[*i + 1] && t[*i + 1][0] == '<')
		{
			if (!t[*i + 2])
				return ("newline");
			if (t[*i + 2] && t[*i + 2][0] == '<')
				return ("<");
		}
	}
	if (t[*i][0] == '>' && t[*i + 1] && t[*i + 1][0] == '|')
		return ("|");
	if (t[*i][0] == '|' && t[*i + 1] && t[*i + 1][0] == '|')
		return ("|");
	return (NULL);
}

/* Processes one token at index i, checking for redirections or arguments.
Handles append/truncate/redirect/pipe tokens and errors.
Updates the command list or prints an error if invalid. */
static t_cmd	*check_redir_pipe(t_cmd *t, char **a[2], int *i, t_cmd_set *p)
{
	char	*err;

	if (!a || !a[0] || !a[0][*i])
		return (*i = -1, put_err("Empty_Pipe", NULL, 2, p), t);
	err = is_invalid_syntax(a[0], i);
	if (err)
		return (*i = -1, put_err("Unexpected_Token", err, 2, p), t);
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
	else
	{
		put_err("Empty_Pipe", NULL, 2, p);
		*i = -1;
	}
	return (t);
}

/* Duplicates the args array and removes quotes from each argument.
Used to clean argument values before building command structures.
Returns the new cleaned array. */
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

/* Parses a list of arguments into a linked list of t_cmd structs.
Handles quote removal, redirections, pipe splitting, and syntax validation.
Returns the built list, or frees and exits early on error. */
static t_list	*parse_cmds(char **args, int i, t_cmd_set *p)
{
	t_list	*cmds[2];
	char	**temp[2];
	t_cmd	*tmp;

	cmds[0] = NULL;
	temp[1] = args_after_quotes_removed(args);
	while (args[++i])
	{
		cmds[1] = ft_lstlast(cmds[0]);
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
		{
			i += args[i][0] == '|';
			ft_lstadd_back(&cmds[0], ft_lstnew(init_cmd()));
			cmds[1] = ft_lstlast(cmds[0]);
		}
		temp[0] = args;
		tmp = cmds[1]->content;
		cmds[1]->content = check_redir_pipe(tmp, temp, &i, p);
		if (i < 0)
			return (free_tmp_lst(cmds[0], args, temp[1]));
	}
	free_array(&temp[1]);
	free_array(&args);
	return (cmds[0]);
}

/* Parses and executes the given arguments.
Uses parse_cmds to build the command list.
Sets the $_ variable and calls exec_cmd_and_wait.
Returns the original pointer after execution. */
void	*parse_nodes(char **args, t_cmd_set *p)
{
	int	status;

	status = 0;
	p->cmds = parse_cmds(split_with_special_chars(args), -1, p);
	if (!p->cmds)
		return (p);
	if (ft_lstsize(p->cmds) == 1 && ((t_cmd *)(p->cmds->content))->args
		&& ft_arr_len(((t_cmd *)(p->cmds->content))->args))
		ft_setenv("_", ((t_cmd *)(p->cmds->content))->args[ft_arr_len(((t_cmd *)
					(p->cmds->content))->args) - 1], p->envp);
	exec_cmd_and_wait(p, status);
	return (p);
}
