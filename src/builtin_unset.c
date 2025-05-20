/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:33:46 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/17 16:33:49 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_unset(t_cmd_set *p, char **args)
{
	int		i;
	char	*tmpstr;
	int		var_index;

	i = -1;
	if (ft_arr_len(args) >= 2)
	{
		while (args[++i])
		{
			if (ft_strlen(args[i]) && args[i][ft_strlen(args[i]) - 1]
				&& args[i][ft_strlen(args[i]) - 1] != '=')
			{
				tmpstr = ft_strjoin(args[i], "=");
				free(args[i]);
				args[i] = tmpstr;
			}
			var_index = find_env_var_index(args[i], p->envp);
			if (var_index != -1)
				ft_array_replace(&p->envp, NULL, var_index);
		}
	}
	return (0);
}
