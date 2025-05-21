/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:43:09 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:43:13 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Used in builtin echo to count the number of characters that are 'n' in an 
argument of function echo.
ex : "echo -nnnn" : countc = 4 = ft_strlen(arg[1]) - 1*/
int	countc(char *s, char c)
{
	int	count;

	if (!s)
		return (0);
	count = 0;
	while (*s)
	{
		if (*s == c)
			count++;
		s++;
	}
	return (count);
}

/* Used in read_heredoc to check if heredoc delimiter (ex EOF)
has any quotes in it, "" doesnt expand $VARIABLES.*/
int	ft_strchrs_idx(const char *s, char *set)
{
	int	i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i] != '\0')
	{
		if (ft_strchr(set, s[i]))
			return (i);
		i++;
	}
	return (-1);
}

/* Searches for the first occurrence of character 'c' in the string 's'.
Returns the index of the character if found.
If the character is '\0', returns the length of the string.
Returns -1 if 's' is NULL or if 'c' is not found in the string. */
int	ft_strchr_idx(const char *s, int c)
{
	unsigned char	c_unsigned;
	int				i;

	i = 0;
	if (!s)
		return (-1);
	c_unsigned = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == c_unsigned)
			return (i);
		i++;
	}
	if (c_unsigned == '\0')
		return (i);
	return (-1);
}
