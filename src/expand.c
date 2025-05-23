/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:30:48 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:36:50 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*tilde_expander(char *str, size_t i, t_cmd_set *p)
{
	char	*home;
	char	*res;
	char	*left;
	char	*right;
	char	*tmp;

	home = ft_getenv("HOME", p->envp);
	if (!home)
		home = ft_strdup("");
	left = ft_substr(str, 0, i);
	right = ft_strdup(str + i + 1);
	tmp = ft_strjoin(left, home);
	res = ft_strjoin(tmp, right);
	free_all(left, right, tmp, str);
	return (res);
}

/*Helper that ensures tilde expansion only happens when ~ 
is at the start of a standalone word.*/
static int	is_delim(char c)
{
	return (c == ' ' || c == '\t' || c == ':' || c == '='
		|| c == '/' || c == '\0');
}

/*Expand environment variables and the lone '~' when it starts a word
and is unquoted.  'quotes[0]' tracks single quotes, quotes[1] double.*/

char	*var_expander(char *str, int quotes[2], t_cmd_set *p)
{
	int		i;
	char	*s[4];

	i = -1;
	str = remove_dollar_quote(str);
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && !quotes[1] && str[i] == '~'
			&& (i == 0 || is_delim(str[i - 1])))
		{
			str = tilde_expander(str, i, p);
			i = -1;
		}
		else if (!quotes[0] && str[i] == '$' && str[i + 1]
			&& (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
			return (var_expander(
					var_or_path_expander(str, ++i, p, s), quotes, p));
	}
	return (str);
}
