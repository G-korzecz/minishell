/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:36:07 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/17 15:36:10 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_chdir(char ***av, char *home, char *oldpwd, t_cmd_set *p)
{
	DIR	*dir;

	dir = NULL;
	if (av[0][1])
		dir = opendir(av[0][1]);
	if (home && !home[0] && !dir)
		put_err_cd("HOME_Not_Set", NULL, 1, p);
	else if (home && (!av[0][1] || (av[0][1][0] == '~')))
		p->status_code = chdir(home) == -1;
	else if (oldpwd && av[0][1] && av[0][1][0] == '-')
		p->status_code = chdir(oldpwd) == -1;
	else if (av[0][1] && access(av[0][1], F_OK) != -1
		&& access(av[0][1], X_OK) == -1)
		put_err_cd("Perm_Denied", av[0][1], 1, p);
	else if (av[0][1] && dir && access(av[0][1], X_OK) == 0)
		chdir(av[0][1]);
	else if (av[0][1] && access(av[0][1], F_OK) == -1)
		put_err_cd("NoFile_NoDir", av[0][1], 1, p);
	else if (av[0][1])
		put_err_cd("NoFile_NoDir", av[0][1], 1, p);
	if (dir)
		closedir(dir);
}

void	check_home_pwd_oldpwd(t_cmd_set *p)
{
	char	*pwd;
	char	*oldpwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_getenv("PWD", p->envp);
	oldpwd = ft_getenv("OLDPWD", p->envp);
	if (!pwd)
		p->envp = ft_setenv("PWD", cwd, p->envp);
	if (!oldpwd)
		p->envp = ft_setenv("OLDPWD", cwd, p->envp);
	free_all(pwd, oldpwd, cwd, NULL);
}

static void	update_pwd_vars(t_cmd_set *p, char *prevpwd)
{
	char	*curpwd;

	p->envp = ft_setenv("OLDPWD", prevpwd, p->envp);
	curpwd = getcwd(NULL, 0);
	if (!curpwd)
		curpwd = ft_strdup("");
	p->envp = ft_setenv("PWD", curpwd, p->envp);
	free_all(curpwd, NULL, NULL, NULL);
}

int	builtin_cd(t_cmd_set *p, char **cmd_args)
{
	char	*home;
	char	*oldpwd;
	char	*prevpwd;

	p->status_code = 0;
	if (cmd_args[1] && cmd_args[2])
		return (put_err(NULL, "cd: too many arguments", 1, p), 1);
	check_home_pwd_oldpwd(p);
	home = ft_getenv("HOME", p->envp);
	if (!home)
		home = ft_strdup("");
	oldpwd = ft_getenv("OLDPWD", p->envp);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	prevpwd = getcwd(NULL, 0);
	if (!prevpwd)
		prevpwd = ft_strdup("");
	ft_chdir(&cmd_args, home, oldpwd, p);
	if (cmd_args[1] && !cmd_args[1][1] && cmd_args[1][0] == '-'
		&& p->status_code == 0)
		ft_putendl_fd(oldpwd, 1);
	update_pwd_vars(p, prevpwd);
	free_all(home, oldpwd, prevpwd, NULL);
	return (p->status_code);
}
