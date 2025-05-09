/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:29:14 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/29 16:30:25 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
