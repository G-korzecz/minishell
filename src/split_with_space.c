/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_space.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:40:04 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:40:09 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Counts the number of tokens the input string, using " " as delimiter.
Skips delimiters inside quotes (single or double).
Uses indexs[0] as index and indexs[1] as token count. */
static int	count_tokens(const char *s, char *c, int indexs[2])
{
	int	in_quote;
	int	quote_char;

	in_quote = 0;
	quote_char = 0;
	while (s[indexs[0]] != '\0')
	{
		if (!ft_strchr(c, s[indexs[0]]))
		{
			indexs[1]++;
			while (s[indexs[0]] && (!ft_strchr(c, s[indexs[0]]) || in_quote))
			{
				if (!quote_char && (s[indexs[0]] == '\"'
						|| s[indexs[0]] == '\''))
					quote_char = s[indexs[0]];
				in_quote = (in_quote + (s[indexs[0]] == quote_char)) % 2;
				quote_char *= in_quote != 0;
				indexs[0]++;
			}
		}
		else
			indexs[0]++;
	}
	return (indexs[1]);
}

/* Extracts substrings from "s", skipping delimiters in quotes,
and stores them into "tmpstr". indexes[0]=scan, [1]=start, [2]=output index.
q[0] single quote, q[1] double quotes. */
static char	**add_to_array1(char **tmpstr, char *s,
	char *set, int indexes[3])
{
	int	s_len;
	int	q[2];

	q[0] = 0;
	q[1] = 0;
	s_len = ft_strlen(s);
	while (s[indexes[0]])
	{
		while (s[indexes[0]] && ft_strchr(set, s[indexes[0]]))
			indexes[0]++;
		indexes[1] = indexes[0];
		while (s[indexes[0]] && (!ft_strchr(set, s[indexes[0]])
				|| q[0] || q[1]))
		{
			q[0] = (q[0] + (!q[1] && s[indexes[0]] == '\'')) % 2;
			q[1] = (q[1] + (!q[0] && s[indexes[0]] == '\"')) % 2;
			indexes[0]++;
		}
		if (indexes[1] >= s_len)
			tmpstr[indexes[2]++] = "\0";
		else
			tmpstr[indexes[2]++] = ft_substr(s, indexes[1],
					indexes[0] - indexes[1]);
	}
	return (tmpstr);
}

/* Splits string "s" by delimiter " " ignoring those inside quotes.
Returns an array of substrings (tokens). */
char	**split_and_ignore_space_if_in_quote(char *s, char *set)
{
	char	**tmpstr;
	int		nwords;
	int		indexes[3];
	int		token_state[2];

	indexes[0] = 0;
	indexes[1] = 0;
	indexes[2] = 0;
	token_state[0] = 0;
	token_state[1] = 0;
	if (!s)
		return (NULL);
	nwords = count_tokens(s, set, token_state);
	tmpstr = malloc((nwords + 1) * sizeof(char *));
	if (tmpstr == NULL)
		return (put_err(NULL, "Malloc failed", 1, NULL), NULL);
	tmpstr = add_to_array1(tmpstr, s, set, indexes);
	tmpstr[nwords] = NULL;
	return (tmpstr);
}
