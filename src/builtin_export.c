/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:35:26 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/17 15:35:35 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Search for a variable in the environment.
Checks if the given assignment string exists in envp.
Returns 1 if found, 0 if not, -1 if the format is invalid. */
int	var_in_envp(char *argv, char **envp, int *index)
{
	int	pos;

	*index = 0;
	pos = ft_strchr_idx(argv, '=');
	if (pos == -1)
		return (-1);
	while (envp[*index])
	{
		if (!ft_strncmp(envp[*index], argv, pos + 1))
			return (1);
		(*index)++;
	}
	return (0);
}

/* Validate a variable name for export.
Ensures the string has a valid format before inserting.
Returns 1 if valid, prints error and returns 0 if invalid. */
int	is_valid_identifier(char *str)
{
	int	j;

	if (!ft_strchr_idx(str, '=') || !str || (str && str[0] == '='))
	{
		ft_printf_fd(2, "mini: export: not a valid identifier\n");
		return (0);
	}
	if ((str[0]) && ft_isdigit(str[0]) && str[0] != '_')
	{
		ft_printf_fd(2, "mini: export: not a valid identifier\n");
		return (0);
	}
	j = 0;
	while (str[j] && str[j] != '=' && str[j] != '_')
	{
		if (!ft_isalnum(str[j]))
		{
			ft_printf_fd(2, "mini: export: not a valid identifier\n");
			return (0);
		}
		j++;
	}
	return (1);
}

/*Validates and inserts or updates environment variables.
Skips invalid entries and prints errors accordingly. */
int	builtin_export(t_cmd_set *p, char **args)
{
	int	i;
	int	pos;
	int	index;

	if (ft_arr_len(args) >= 2)
	{
		i = 1;
		while (args[i])
		{
			if (!is_valid_identifier(args[i]))
				return (1);
			pos = var_in_envp(args[i], p->envp, &index);
			if (pos == 1)
			{
				if (p->envp[index])
					free(p->envp[index]);
				p->envp[index] = ft_strdup(args[i]);
			}
			else if (!pos)
				p->envp = ft_array_insert(p->envp, args[i]);
			i++;
		}
	}
	return (0);
}
