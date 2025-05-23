/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 23:22:46 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/22 23:29:48 by gkorzecz         ###   ########.fr       */
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

char	*var_or_path_expander(char *str, int i, t_cmd_set *p, char *s[4])
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

static void	update_quote(int *in_squote, int *in_dquote, char c)
{
	if (c == '\'' && *in_dquote == 0)
	{
		if (*in_squote == 0)
			*in_squote = 1;
		else
			*in_squote = 0;
	}
	else if (c == '"' && *in_squote == 0)
	{
		if (*in_dquote == 0)
			*in_dquote = 1;
		else
			*in_dquote = 0;
	}
}

/* Remove the character at index pos and return a new malloc-ated string.*/
static char	*remove_char_at(char *str, size_t pos)
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

/* Delete the ‘$’ that introduces $'ansi' or $"gettext" quoting
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
		update_quote(&in_squote, &in_dquote, str[i]);
		if (in_squote == 0 && in_dquote == 0 && str[i] == '$'
			&& (str[i + 1] == '\'' || str[i + 1] == '"'))
			return (remove_char_at(str, i));
		i++;
	}
	return (str);
}
