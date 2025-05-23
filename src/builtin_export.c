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

static int	var_in_envp(char *arg, char **envp, int *idx)
{
	int	pos;

	*idx = 0;
	pos = ft_strchr_idx(arg, '=');
	if (pos == -1)
		return (-1);
	while (envp[*idx])
	{
		if (!ft_strncmp(envp[*idx], arg, pos + 1))
			return (1);
		(*idx)++;
	}
	return (0);
}

static int	is_valid_identifier(char *str)
{
	int		i;
	char	*err_msg;

	err_msg = "mini: export: not a valid identifier\n";
	if (!str || str[0] == '=')
		return (ft_printf_fd(2, "%s", err_msg), 0);
	if (ft_isdigit(str[0]) || str[0] == '+')
		return (ft_printf_fd(2, "%s", err_msg), 0);
	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '_')
	{
		if (!ft_isalnum(str[i]))
		{
			return (ft_printf_fd(2, "%s", err_msg), 0);
		}
		i++;
	}
	return (1);
}

static void	sort_env(char **arr, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export(char **envp)
{
	int		i;
	int		pos;
	char	*key;

	sort_env(envp, ft_arr_len(envp));
	i = 0;
	while (envp[i])
	{
		pos = ft_strchr_idx(envp[i], '=');
		if (pos == -1)
			ft_printf_fd(1, "declare -x %s\n", envp[i]);
		else
		{
			key = ft_substr(envp[i], 0, pos);
			ft_printf_fd(1,
				"declare -x %s=\"%s\"\n",
				key,
				envp[i] + pos + 1);
			free(key);
		}
		i++;
	}
}

int	builtin_export(t_cmd_set *p, char **args)
{
	int	i;
	int	pos;
	int	idx;

	p->status_code = 0;
	if (!args[1])
		return (print_export(p->envp), 0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
			return (1);
		pos = var_in_envp(args[i], p->envp, &idx);
		if (pos == 1)
		{
			free(p->envp[idx]);
			p->envp[idx] = ft_strdup(args[i]);
		}
		else if (pos == 0)
			p->envp = ft_array_insert(p->envp, args[i]);
		i++;
	}
	return (0);
}
