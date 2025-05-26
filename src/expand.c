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

/* Expand the '~' special variable as HOME.
Get what comes before (left) and after (right) append to both end of HOME
i is the index of tilde.*/
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

/* Find both exit status for $? or variable value ($USER, $HOME....)
if variable doesnt exist, return ""*/
char	*find_substitution(char first, char *var, t_cmd_set *p)
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

/* Actual recursive expander, first check the lenght of the variable to send
a copie to "find_substitution" that will replace it with its actual value
i.e. exit code if '?' or var value for '$'.
Reconstruct the string via append left and right to the variable value
call var_expander on the result with quoting value are reset to 0.
Recursion stop when "else if" not entered.*/
char	*expand_variable(char *str, int i, t_cmd_set *p, char *s[3])
{
	int		j;
	char	*res;
	char	*left;
	char	*right;

	j = 0;
	if (str[i] == '?')
		j = 1;
	else
		while (str[i + j] && (ft_isalnum(str[i + j]) || str[i + j] == '_'))
			j++;
	s[0] = ft_substr(str, i, j);
	s[1] = find_substitution(str[i], s[0], p);
	left = ft_substr(str, 0, i - 1);
	right = ft_strdup(&str[i + j]);
	s[2] = ft_strjoin(left, s[1]);
	res = ft_strjoin(s[2], right);
	free_all(left, right, s[1], s[2]);
	free(str);
	return (var_expander(res, (int [2]){0, 0}, p));
}

/* Main recursive loop for expanding var, first remove useless $ before quotes
quotes[0] : inside single quote
quotes[1] : inside double quote
tilde_expander for HOME (only if not in singe/double quotes)
then if it finds $ that is followed by a valid var, the recursion begin
until there is nor more variable to expand.*/
char	*var_expander(char *str, int quotes[2], t_cmd_set *p)
{
	int		i;
	char	*s[3];

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
					expand_variable(str, ++i, p, s), quotes, p));
	}
	return (str);
}
