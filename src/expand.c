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

static char	*find_substitution(char first, char *var, t_cmd_set *p)
{
	char	*val;

	val = NULL;
	if (first == '?')
	{
		if (g_exit_status > 0)
		{
			p->status_code = g_exit_status;
			g_exit_status = 0;
		}
		val = ft_itoa(p->status_code);
	}
	else if (var)
		val = ft_getenv(var, p->envp);
	free(var);
	if (!val)
		val = ft_strdup("");
	return (val);
}

static char	*var_or_path_expander(char *str, int i, t_cmd_set *p, char *s[4])
{
	int		j;
	char	*res;
	char	*prefix;
	char	*suffix;

	j = 0;
	if (str[i] == '?')
		j = 1;
	else
		while (str[i + j] && (ft_isalnum(str[i + j]) || str[i + j] == '_'))
			j++;
	s[0] = ft_substr(str, i, j);
	s[1] = find_substitution(str[i], s[0], p);
	prefix = ft_substr(str, 0, i - 1);
	suffix = ft_strdup(&str[i + j]);
	s[2] = ft_strjoin(prefix, s[1]);
	res = ft_strjoin(s[2], suffix);
	free_all(prefix, suffix, s[1], s[2]);
	free(str);
	return (var_expander(res, (int [2]){0, 0}, p));
}

static char	*remove_dollar_quote(char *str)
{
	size_t	i;
	char	*left;
	char	*right;
	char	*tmp;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$'
			&& (str[i + 1] == '"' || str[i + 1] == '\''))
		{
			left = ft_substr(str, 0, i);
			right = ft_strdup(str + i + 1);
			tmp = ft_strjoin(left, right);
			free(left);
			free(right);
			free(str);
			return (tmp);
		}
		i++;
	}
	return (str);
}

char	*var_expander(char *str, int quotes[2], t_cmd_set *p)
{
	char	*tmp[2];
	char	*s[4];
	int		i;

	i = -1;
	str = remove_dollar_quote(str);
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && str[i] == '$' && str[i + 1]
			&& (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
		{
			tmp[1] = var_or_path_expander(str, ++i, p, s);
			tmp[0] = tmp[1];
			tmp[1] = ft_strjoin(tmp[1], "");
			free(tmp[0]);
			return (var_expander(tmp[1], quotes, p));
		}
	}
	return (str);
}
