/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:58:29 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/14 00:03:20 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_pwd(void)
{
	char	current_wd[PATH_MAX];

	if (getcwd(current_wd, PATH_MAX) != NULL)
	{
		printf("%s'n", current_wd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
