/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 00:51:04 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/15 00:51:32 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_exit(t_list *cmd, int *is_exit, t_cmd_set *p)
{
	long long int	status[2];
	char			**args;

	args = ((t_cmd *)cmd->content)->args;
	*is_exit = !cmd->next;
	if (*is_exit)
		printf("exit\n");
	if (!args || !args[1])
		free_exit(p, 0, NULL);
	status[1] = ft_atoi(args[1]);
	if (args[1][0] == '\0' || (status[1] < 0 && ft_strlen(args[1]) > 18)
		|| (status[1] == 0 && args[1][0] != '0'))
	{
		*is_exit = 1;
		ft_putstr_fd("mini: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		free_exit(p, (2 + status[1]) & 255, ": numeric argument required");
	}
	else if (args[2])
	{
		*is_exit = 0;
		put_err(NULL, "mini: exit: too many arguments", 1, 0);
	}
	if (*is_exit)
		free_exit(p, status[1] & 255, NULL);
}

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

/* checks if the variable name is valid, it must not begin with digit
	all characters must be alphanumeric, except "_"  */
int	is_valid_identifier(char *str)
{
	int	j;

	if (!ft_strchr_idx(str, '=') || !str || (str && str[0] == '='))
	{
		ft_putstr_fd("mini: export: not a valid identifier\n", 2);
		return (0);
	}
	if ((str[0]) && ft_isdigit(str[0]) && str[0] != '_')
	{
		ft_putstr_fd("mini: export: not a valid identifier\n", 2);
		return (0);
	}
	j = 0;
	while (str[j] && str[j] != '=' && str[j] != '_')
	{
		if (!ft_isalnum(str[j]))
		{
			ft_putstr_fd("mini: export: not a valid identifier\n", 2);
			return (0);
		}
		j++;
	}
	return (1);
}

/* loops through the given arguments, calls the is_valid_identifier func
	to check if it's a valid variable name.
	calls var_in_envp to check if it already exists in p->envp, 
	if exists, frees the old value and saves the new value
	if not exists, calls ft_array_insert to add the new variable
	if no argument is given, does nothing as it's undefined behavior */
int	ft_export(t_cmd_set *p, char **args)
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

void	ft_chdir(char ***av, char *home, char *oldpwd, t_cmd_set *p)
{
	DIR	*dir;

	dir = NULL;
	if (!ft_strncmp(av[0][0], "cd", ft_strlen(av[0][0])) && av[0][1])
		dir = opendir(av[0][1]);
	if (home && !home[0] && !dir)
		put_err(NULL, "mini: HOME not set", 1, p);
	else if (home && ((!dir && !av[0][1]) || (av[0][1] && av[0][1][0]
		&& av[0][1][0] == '~')))
		p->status_code = chdir(home) == -1;
	else if (oldpwd && (!av[0][1] || (av[0][1] && av[0][1][0]
		&& av[0][1][0] == '-')))
		p->status_code = chdir(oldpwd) == -1;
	else if (av[0][1] && dir && access(av[0][1], F_OK) != -1)
		chdir(av[0][1]);
	else if (!ft_strncmp(av[0][0], "cd", ft_strlen(av[0][0])) && av[0][1]
		&& access(av[0][1], F_OK) == -1)
		put_err("NoFile_NoDir", av[0][1], 1, p);
	else if (!ft_strncmp(av[0][0], "cd", ft_strlen(av[0][0]))
		&& !dir && av[0][1])
		put_err("Not_Directory", av[0][1], 1, p);
	if (dir)
		closedir(dir);
}

/* ensure that PWD and OLDPWD exist in the env.
	first it gets the current WD and the variables.
	if one of the variables is not set, it sets to the current directory */
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

/* this is the builtin that is actually called.
	it verifies, that there is only one argument.
	it ensures that PWD and OLDPWD are set in the env.
	it ensures that at least "" home and oldpwd exist.
	it uses ft_chdir to do the actual change of directory.
	then it sets the PWD, frees memory, and returns. */
int	ft_cd(t_cmd_set *p, char **cmd_args)
{
	char	*home;
	char	*oldpwd;
	char	*newpwd;

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
	newpwd = getcwd(NULL, 0);
	ft_chdir(&cmd_args, home, oldpwd, p);
	p->envp = ft_setenv("OLDPWD", newpwd, p->envp);
	free_all(newpwd, NULL, NULL, NULL);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		newpwd = ft_strdup("");
	p->envp = ft_setenv("PWD", newpwd, p->envp);
	free_all(home, oldpwd, newpwd, NULL);
	return (p->status_code);
}

int	ft_unset(t_cmd_set *p, char **args)
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