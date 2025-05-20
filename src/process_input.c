/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:39:27 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:39:30 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Checks if the current character is "${" and not inside quotes.
Updates quote states. Used in remove_curly_brackets(). */
int	update_quotes_chk_curly_bracket(int *quotes, char ch, int i[3], char **s)
{
	quotes[0] = (quotes[0] + (!quotes[1] && ch == '\'')) % 2;
	quotes[1] = (quotes[1] + (!quotes[0] && ch == '\"')) % 2;
	if (!quotes[0] && !quotes[1] && (*s)[i[0]] && (*s)[i[0]] == '$' && (*s)[i[0]
		+ 1] && (*s)[i[0] + 1] == '{' && (*s)[i[0] + 2])
		return (1);
	else
		return (0);
}

/* Replaces ${VAR} with $VAR for correct expansion.
Handles content inside quotes and reconstructs the string. */
void	remove_curly_brackets(char **s, int i[3], int quotes[2], char *tmp[3])
{
	while (*s && i[0] < (int)ft_strlen(*s) && (*s)[++i[0]])
	{
		if (update_quotes_chk_curly_bracket(quotes, (*s)[i[0]], i, s))
		{
			i[1] = i[0] + 2;
			i[2] = i[1];
			while ((*s)[i[1]] && (*s)[i[1]] != ' ' && (*s)[i[1]] != '}')
				i[1]++;
			if ((*s)[i[1]] == '}')
				tmp[2] = ft_substr(*s, i[2], i[1] - i[2]);
			else
				tmp[2] = ft_substr(*s, i[2] - 1, i[1] - i[2] + 1);
			tmp[0] = ft_substr(*s, 0, i[0]);
			tmp[1] = ft_strjoin(tmp[0], "$");
			free(tmp[0]);
			tmp[0] = ft_strjoin(tmp[1], tmp[2]);
			free_all(tmp[1], tmp[2], NULL, NULL);
			tmp[1] = ft_strjoin(tmp[0], "");
			free(tmp[0]);
			tmp[0] = ft_strdup(&(*s)[i[1] + 1]);
			free_all(*s, NULL, NULL, NULL);
			*s = ft_strjoin(tmp[1], tmp[0]);
			free_all(tmp[0], tmp[1], NULL, NULL);
		}
	}
}

/* Checks for heredoc operator (<<) outside of quotes.
Returns 1 if a valid heredoc is detected, 0 if syntax error. */
int	update_quotes_chk_heredoc(int *quo, char ch, int i[3], char **s)
{
	quo[0] = (quo[0] + (!quo[1] && ch == '\'')) % 2;
	quo[1] = (quo[1] + (!quo[0] && ch == '\"')) % 2;
	if (!quo[0] && !quo[1] && (*s)[i[0]] && (*s)[i[0]] == '<' && (*s)[i[0] + 1]
		&& (*s)[i[0] + 1] == '<' && (*s)[i[0] + 2] && (*s)[i[0] + 2] == '<')
	{
		put_err("syntax error near unexpected token `<'", NULL, 2, NULL);
		return (0);
	}
	else if (!quo[0] && !quo[1] && (*s)[i[0]] && (*s)[i[0]] == '<'
		&& (*s)[i[0] + 1] && (*s)[i[0] + 1] == '<' && (*s)[i[0] + 2])
		return (1);
	else
		return (0);
}

/* Processes the user input: validates quotes, tokenizes input,
parses commands, and prepares them for execution. */
void	*process_input(char *input, t_cmd_set *p)
{
	char	**a;
	int		i[3];
	int		quotes[2];

	i[0] = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	if (!input)
		return (NULL);
	if (input[0] != '\0')
		add_history(input);
	handle_input(&input, i, quotes, p);
	if (check_unclosed_quotes(input))
	{
		put_err("Unclosed_Quote", NULL, 2, p);
		return (p);
	}
	a = split_and_ignore_space_if_in_quote(input, " ");
	if (!a)
		free_exit(p, 1, "Unexpected Error");
	p = parse_nodes(a, p);
	free_all(input, NULL, NULL, NULL);
	if (p && p->cmds)
		ft_lstclear(&p->cmds, free_lst);
	return (p);
}

/* Scans the input string for heredoc operators (<<) outside quotes.
Wraps the delimiter with single quotes to prevent expansion later.
Modifies the input string in-place using tmp for partial rebuilding. */
void	process_heredoc(char **s, int i[3], int quotes[2], char *tmp[3])
{
	while (*s && i[0] < (int)ft_strlen(*s) && (*s)[++i[0]])
	{
		if (update_quotes_chk_heredoc(quotes, (*s)[i[0]], i, s))
		{
			i[1] = i[0] + 2;
			while ((*s)[i[1]] && (ft_strchr(" \\t", (*s)[i[1]])))
				i[1]++;
			i[2] = i[1];
			while ((*s)[i[1]] && (*s)[i[1]] != ' ' && (*s)[i[1]] != '\t')
				i[1]++;
			tmp[2] = ft_substr(*s, i[2], i[1] - i[2]);
			tmp[0] = ft_substr(*s, 0, i[0]);
			tmp[1] = ft_strjoin(tmp[0], "<<'");
			free(tmp[0]);
			tmp[0] = ft_strjoin(tmp[1], tmp[2]);
			free_all(tmp[1], tmp[2], NULL, NULL);
			tmp[1] = ft_strjoin(tmp[0], "'");
			free(tmp[0]);
			tmp[0] = ft_strdup(&(*s)[i[1]]);
			free_all(*s, NULL, NULL, NULL);
			*s = ft_strjoin(tmp[1], tmp[0]);
			free_all(tmp[0], tmp[1], NULL, NULL);
		}
	}
}
