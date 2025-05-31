/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwang <cwang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:50:23 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/28 16:24:38 by cwang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

/* Replaces ${VAR} with $VAR for correct expansion later.
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
Returns 1 if a valid heredoc is detected, 0 if syntax error. 
Note that it is for safety as syntax error are already catching most 
of it.*/
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
	else if (!quo[0] && !quo[1] && (*s)[i[0]] && (*s)[i[0]] == '<' && (*s)[i[0]
			+ 1] && (*s)[i[0] + 1] == '<' && (*s)[i[0] + 2])
		return (1);
	else
		return (0);
}

/* Scans the input string for heredoc operators (<<) outside quotes.
Put the delimiter between single quotes to prevent expansion later.
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

/* Handles modifications to the input string before parsing.
Processes heredoc delimiters and expands ${VAR} and $VAR.
Updates the input string in-place with all applied changes. */
void	handle_input(char **input, int i[3], int quotes[2], t_cmd_set *p)
{
	char	*tmp[3];

	if (ft_strnstr(*input, "<<", ft_strlen(*input)))
		process_heredoc(input, i, quotes, tmp);
	if (ft_strnstr(*input, "${", ft_strlen(*input)))
		remove_curly_brackets(input, i, quotes, tmp);
	*input = var_expander(*input, quotes, p);
}
