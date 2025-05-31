/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwang <cwang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 23:22:46 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/29 12:21:45 by cwang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*Make sure $ before quotes is only removed if it's not inside other quotes.*/
void	track_quotes(int *in_squote, int *in_dquote, char c)
{
	if (c == '\'' && *in_dquote == 0)
	{
		if (*in_squote == 0)
			*in_squote = 1;
		else
			*in_squote = 0;
	}
	else if (c == '\"' && *in_squote == 0)
	{
		if (*in_dquote == 0)
			*in_dquote = 1;
		else
			*in_dquote = 0;
	}
}

/* Remove the character at index pos and return a new malloc string.*/
char	*remove_char_at(char *str, size_t pos)
{
	char	*left;
	char	*right;
	char	*tmp;

	left = ft_substr(str, 0, pos);
	right = ft_strdup(str + pos + 1);
	tmp = ft_strjoin(left, right);
	free(left);
	free(right);
	free(str);
	return (tmp);
}

/* Delete the ‘$’ before quotes.
echo $"USER" = USER and not $USER
only when we are *outside* any existing quote context.*/
char	*remove_dollar_quote(char *str)
{
	size_t	i;
	int		in_squote;
	int		in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (str && str[i])
	{
		track_quotes(&in_squote, &in_dquote, str[i]);
		if (in_squote == 0 && in_dquote == 0 && str[i] == '$'
			&& (str[i + 1] == '\'' || str[i + 1] == '"'))
			return (remove_char_at(str, i));
		i++;
	}
	return (str);
}
