/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:08:28 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/17 16:08:55 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	allocate_env_vars_if_envp_empty(t_cmd_set *p)
{
	size_t	i;
	char	*default_env[6];

	default_env[0] = "PATH=/usr/bin:/bin";
	default_env[1] = "HOME=/";
	default_env[2] = "PWD=/";
	default_env[3] = "OLDPWD=/";
	default_env[4] = "SHLVL=1";
	default_env[5] = "_=./minishell";
	i = 0;
	while (i < 6)
	{
		p->envp[i] = ft_strdup(default_env[i]);
		if (p->envp[i] == NULL)
			return (0);
		i++;
	}
	return (1);
}

static void	init_reset(t_cmd_set *p, char **envp)
{
	p->cmds = NULL;
	p->input_text = NULL;
	g_exit_status = 0;
	p->status_code = 0;
	p->pid_of_lst_cmd = 0;
	if (envp && *envp)
		p->envp = ft_dup_array(envp);
	if (!p->envp || !(envp && *envp))
	{
		p->envp = (char **)ft_calloc(6, sizeof(char *));
		if (p->envp != NULL)
		{
			if (!allocate_env_vars_if_envp_empty(p))
			{
				ft_putendl_fd("Error: Failed to duplicate env variables", 2);
				free_exit(p, 1, NULL);
			}
		}
		else
		{
			ft_putendl_fd("Error: Failed to allocate memory for p->envp", 2);
			free_exit(p, 1, NULL);
		}
	}
}

void	init(t_cmd_set *p, char **envp, char **argv, int argc)
{
	char	*path;
	char	*cwd;
	char	*shlvl;
	char	*num;

	if (argc > 1 || argv[1])
		ft_putendl_fd("Arguments have been ignored", 2);
	init_reset(p, envp);
	path = ft_getenv("PATH", p->envp);
	if (path == NULL)
		p->envp = ft_setenv("PATH", "/usr/bin:/bin", p->envp);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup("/");
	p->envp = ft_setenv("PWD", cwd, p->envp);
	shlvl = ft_getenv("SHLVL", p->envp);
	if (!shlvl || ft_atoi(shlvl) <= 0 || ft_atoi(shlvl) == 2147483647)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(shlvl) + 1);
	if (num == NULL)
		num = ft_strdup("");
	p->envp = ft_setenv("SHLVL", num, p->envp);
	free_all(num, cwd, shlvl, path);
}
