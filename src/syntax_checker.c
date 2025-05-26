/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:16:17 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/24 13:16:42 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_redir(char c)
{
	char	c2;

	c2 = c;
	if (c2 == '<' || c2 == '>')
		return (1);
	return (0);
}

static char	*pipe_error(char **tok, int idx)
{
	char	*next;

	next = tok[idx + 1];
	if (tok[idx][0] == '|' && idx == 0)
		return ("|");
	if (tok[idx][0] == '|' && (!next || next[0] == '\0'))
		return ("eof");
	if (tok[idx][0] == '|' && next && next[0] == '|')
		return ("|");
	if (is_redir(tok[idx][0]) && next && next[0] == '|')
		return ("|");
	return (NULL);
}

static char	*redir_error(char **tok, int idx)
{
	char	c;
	int		n;
	char	*next;

	c = tok[idx][0];
	n = 0;
	next = tok[idx + 1];
	while (tok[idx + n] && tok[idx + n][0] == c)
		n++;
	if (c == '<' && n == 3)
		return ("newline");
	if (n > 2 && c == '>')
		return (">");
	if (n > 2)
		return ("<");
	if (n == 2 && (!next || next[0] == '\0' || next[0] == '|'))
		return ("newline");
	if (n == 1 && (!next || is_redir(next[0]) || next[0] == '|'))
		return ("newline");
	if (c == '>' && next && next[0] == '<')
		return ("<");
	if (c == '<' && next && next[0] == '>')
		return ("newline");
	return (NULL);
}

char	*is_invalid_syntax(char **tok, int *i)
{
	char	*err;
	char	c;

	if (!tok[*i])
		return ("newline");
	c = tok[*i][0];
	err = pipe_error(tok, *i);
	if (err != NULL)
		return (err);
	if (is_redir(c))
	{
		err = redir_error(tok, *i);
		if (err != NULL)
			return (err);
	}
	return (NULL);
}
