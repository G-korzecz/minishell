/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:31:19 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:31:26 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*Duplicate dir into temp_dir, append "/" to it
then append the command.
free temp and return cmd_path*/
static char	*build_cmd_path(const char *dir, const char *cmd)
{
	char	*temp_dir;
	char	*temp;
	char	*cmd_path;

	temp_dir = ft_strdup(dir);
	if (!temp_dir)
		return (NULL);
	temp = ft_strjoin(temp_dir, "/");
	free(temp_dir);
	if (!temp)
		return (NULL);
	cmd_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}

/* Loops through env variable to check if the command is in a path of $PATH
Call build command path each time (append '/' cmd)
so it search for access to cmd_path and if execution is possible.
Free the path each time to check for next one.
if nothing is found in $PATH fall back to "." which is current directory.
lately if nothing found return NULL and launch error no_cmd in process_check*/
char	*find_command(char **env_path, char *cmd, char *cmd_path)
{
	int		i;

	i = -1;
	while (env_path && env_path[++i])
	{
		free_all(cmd_path, NULL, NULL, NULL);
		cmd_path = build_cmd_path(env_path[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
	}
	free_all(cmd_path, NULL, NULL, NULL);
	cmd_path = build_cmd_path(".", cmd);
	if (cmd_path && access(cmd_path, X_OK) == 0)
		return (cmd_path);
	free_all(cmd_path, NULL, NULL, NULL);
	return (NULL);
}

/* check a naked command which is not a builtin (type ls) 
if empty (ex : "   ") error :No_CMD
Split the $PATH variable into an array:
[0] : /usr/bin
[1] : /usr/sbin
....
call find command
if nothing found and last command error code 127
else 0 (executed in child)*/
static void	process_checks(t_list *cmd, char *path, t_cmd_set *p, char ***s)
{
	t_cmd	*n;

	n = cmd->content;
	if (n->args && n->args[0] && n->args[0][0] == '\0')
	{
		put_err("No_Cmd", n->args[0], 127, p);
		return ;
	}
	if (!is_builtin(n) && n->args && !ft_strchr("/.~", n->args[0][0]))
	{
		*s = ft_split(path, ':');
		n->cmd_path = find_command(*s, *n->args, n->cmd_path);
		if ((!n->cmd_path || !n->args[0]) && !cmd->next && n->in_fd != -1)
			put_err("No_Cmd", n->args[0], 127, p);
		else if ((!n->cmd_path || !n->args[0]) && n->in_fd != -1)
			put_err("No_Cmd", n->args[0], 0, p);
	}
}

/* Try opendir to check if the cmd entered is a directory
if not and contain a '/' store a copy into cmd_path
and replace arg[0] with the command /bin/ls -> ls
if not fallback to process_check.
if the cmd is not a directory, dir is null and conditions in
find_cmd_path will be false.*/
static DIR	*cmd_checks(t_list *cmd, char ***s, char *path, t_cmd_set *p)
{
	t_cmd	*n;
	DIR		*dir;

	n = cmd->content;
	dir = NULL;
	if (n && n->args)
		dir = opendir(n->args[0]);
	if (n && n->args && ft_strchr(*n->args, '/') && !dir)
	{
		*s = ft_split(*n->args, '/');
		n->cmd_path = ft_strdup(*n->args);
		free(n->args[0]);
		n->args[0] = ft_strdup((*s)[ft_arr_len(*s) - 1]);
	}
	else if (!dir)
		process_checks(cmd, path, p, s);
	free_all(path, NULL, NULL, NULL);
	return (dir);
}

/*Main cmd_path_checker.
Throw errors in function of cmd_check.
First get the env variable PATH from p->envp
Close dir if opened before (non NULL).*/
void	find_cmd_path(t_cmd_set *p, t_list *cmd, char **s, char *path)
{
	t_cmd	*n;
	DIR		*dir;

	n = cmd->content;
	path = ft_getenv("PATH", p->envp);
	if (!path || !*path)
		path = ft_strdup("/");
	dir = cmd_checks(cmd, &s, path, p);
	if (!is_builtin(n) && n && n->args && dir && (n->args[0][0]
		&& !ft_strchr("/.~", n->args[0][0])))
		put_err(NULL, "command not found", 127, p);
	else if (!is_builtin(n) && n && n->args && dir && n->args[0][0]
			&& (ft_strchr("/.~", n->args[0][0])))
		put_err("Is_Directory", *n->args, 126, p);
	else if (!is_builtin(n) && n && n->cmd_path
		&& access(n->cmd_path, F_OK) == -1)
		put_err("NoFile_NoDir", n->cmd_path, 127, p);
	else if (!is_builtin(n) && n && n->cmd_path
		&& access(n->cmd_path, X_OK) == -1)
		put_err("Perm_Denied", n->cmd_path, 126, p);
	if (dir)
		closedir(dir);
	if (cmd->next != NULL)
		p->status_code = 0;
	free_array(&s);
}
