/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:50:23 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/20 21:50:26 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

/* Checks if a string has unclosed single or double quotes.
Returns 1 if unclosed, 0 otherwise. */
int	check_unclosed_quotes(const char *s)
{
	int	squote;
	int	dquote;
	int	i;

	squote = 0;
	dquote = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' && dquote == 0)
			squote = !squote;
		else if (s[i] == '"' && squote == 0)
			dquote = !dquote;
		i++;
	}
	return (squote || dquote);
}
