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

int	var_in_envp(char *argv, char **envp, int ij[2])
{
	int	pos;

	ij[1] = 0;
	pos = ft_strchr_idx(argv, '=');
	if (pos == -1)
		return (-1);
	while (envp[ij[1]])
	{
		if (!ft_strncmp(envp[ij[1]], argv, pos + 1))
			return (1);
		ij[1]++;
	}
	return (0);
}

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

int	builtin_export(t_cmd_set *p, char **args)
{
	int		ij[2];
	int		pos;

	if (ft_arr_len(args) >= 2)
	{
		ij[0] = 1;
		while (args[ij[0]])
		{
			if (!is_valid_identifier(args[ij[0]]))
				return (1);
			pos = var_in_envp(args[ij[0]], p->envp, ij);
			if (pos == 1)
			{
				if (p->envp[ij[1]])
					free(p->envp[ij[1]]);
				p->envp[ij[1]] = ft_strdup(args[ij[0]]);
			}
			else if (!pos)
				p->envp = ft_array_insert(p->envp, args[ij[0]]);
			ij[0]++;
		}
	}
	return (0);
}
