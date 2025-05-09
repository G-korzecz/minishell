/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:09 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:42:43 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Returns the number of elements in a NULL-terminated string array.
Used for envp or other argument arrays. */
int	ft_arr_len(char **m)
{
	int	i;

	i = 0;
	if (!m)
		return (i);
	while (m && m[i])
		i++;
	return (i);
}

/* Creates a deep copy of a NULL-terminated string array.
Each string is duplicated. Returns the new array or NULL on failure. */
char	**ft_dup_array(char **arr_ptr)
{
	char	**arr_copy;
	int		n_rows;
	int		i;

	i = 0;
	n_rows = ft_arr_len(arr_ptr);
	arr_copy = malloc(sizeof(char *) * (n_rows + 1));
	if (!arr_copy)
		return (NULL);
	while (arr_ptr[i])
	{
		arr_copy[i] = ft_strdup(arr_ptr[i]);
		if (!arr_copy[i])
		{
			free_array(&arr_copy);
			return (NULL);
		}
		i++;
	}
	arr_copy[i] = NULL;
	return (arr_copy);
}

/* Inserts a new string into a string array (like adding an env var).
Returns a new array with the string added at the end.
Frees the original input array. */
char	**ft_array_insert(char **in, char *newstr)
{
	char	**out;
	int		len;
	int		i;

	i = -1;
	out = NULL;
	if (!newstr)
		return (in);
	len = ft_arr_len(in);
	out = ft_calloc(1, sizeof(char *) * (len + 2));
	if (!out)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	out[len + 1] = NULL;
	while (++i < len)
	{
		out[i] = ft_strdup(in[i]);
		if (!out[i])
		{
			free_array(&in);
			free_array(&out);
		}
	}
	out[i] = ft_strdup(newstr);
	free_array(&in);
	return (out);
}

/* Replaces the n-th element of a string array with another string array.
Inserts all elements of 'small' in place of 'big[n]'.
Frees the original array and returns the new one. */
char	**ft_array_replace(char ***big, char **small, int n)
{
	char	**tmpstr;
	int		i[3];

	i[0] = -1;
	i[1] = -1;
	i[2] = -1;
	if (!big || !*big || n < 0 || n >= ft_arr_len(*big))
		return (NULL);
	tmpstr = ft_calloc(ft_arr_len(*big) + ft_arr_len(small), sizeof(char *));
	if (!tmpstr)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	while (tmpstr && big[0][++i[0]])
	{
		if (i[0] != n)
			tmpstr[++i[2]] = ft_strdup(big[0][i[0]]);
		else
		{
			while (small && small[++i[1]])
				tmpstr[++i[2]] = ft_strdup(small[i[1]]);
		}
	}
	free_array(big);
	*big = tmpstr;
	return (*big);
}
