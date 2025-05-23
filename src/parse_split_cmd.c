/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_split_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:38:34 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:38:38 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof * cmd);
	if (!cmd)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	cmd->args = NULL;
	cmd->cmd_path = NULL;
	cmd->in_fd = 0;
	cmd->out_fd = 1;
	return (cmd);
}

static int	size_to_malloc(char *s, char *set, int count)
{
	int	q[2];
	int	i;

	i = 0;
	q[0] = 0;
	q[1] = 0;
	while (s && s[i] != '\0')
	{
		count++;
		if (!ft_strchr(set, s[i]))
		{
			while ((!ft_strchr(set, s[i]) || q[0] || q[1]) && s[i] != '\0')
			{
				q[0] = (q[0] + (!q[1] && s[i] == '\'')) % 2;
				q[1] = (q[1] + (!q[0] && s[i] == '\"')) % 2;
				i++;
			}
			if (q[0] || q[1])
				return (-1);
		}
		else
			i++;
	}
	return (count);
}

static char	**ft_add_to_array(char **tmpstr, char *s, char *set, int i[3])
{
	int	q[2];

	q[0] = 0;
	q[1] = 0;
	while (s && s[i[0]] != '\0')
	{
		i[1] = i[0];
		if (!ft_strchr(set, s[i[0]]))
		{
			while ((!ft_strchr(set, s[i[0]]) || q[0] || q[1]) && s[i[0]])
			{
				q[0] = (q[0] + (!q[1] && s[i[0]] == '\'')) % 2;
				q[1] = (q[1] + (!q[0] && s[i[0]] == '\"')) % 2;
				i[0]++;
			}
		}
		else
			i[0]++;
		tmpstr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	return (tmpstr);
}

char	**ft_split_with_pipe_or_redir_char(char const *s, char *set)
{
	char	**tmp;
	int		word_count;
	int		i[3];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	if (!s)
		return (NULL);
	word_count = size_to_malloc((char *)s, set, 0);
	if (word_count == -1)
		return (NULL);
	tmp = malloc((word_count + 1) * sizeof(char *));
	if (tmp == NULL)
		return (NULL);
	tmp = ft_add_to_array(tmp, (char *)s, set, i);
	tmp[word_count] = NULL;
	return (tmp);
}

char	**split_with_special_chars(char **args)
{
	char	**subsplit;
	int		i[3];

	i[2] = -1;
	while (args && args[++i[2]])
	{
		subsplit = ft_split_with_pipe_or_redir_char(args[i[2]], "<|>");
		ft_array_replace(&args, subsplit, i[2]);
		i[2] += ft_arr_len(subsplit) - 1;
		free_array(&subsplit);
	}
	return (args);
}
