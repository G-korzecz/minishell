/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:43:42 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:43:45 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Frees the content of a t_cmd struct inside a linked list node.
Frees arguments and cmd_path, and closes fds if they are valid and open. */
void	free_lst(void *content)
{
	t_cmd	*node;

	node = content;
	free_array(&node->args);
	if (node->cmd_path)
		free(node->cmd_path);
	if (node->in_fd != 0 && node->in_fd != -1)
		close(node->in_fd);
	if (node->out_fd && node->out_fd != 1 && node->out_fd != -1)
		close(node->out_fd);
	if (node)
		free(node);
}

/* Frees a NULL-terminated array of strings and sets pointer to NULL.
Takes a triple pointer to free both the content and container. */
void	free_array(char ***m)
{
	int	i;

	i = 0;
	if (!m || !*m || !**m)
		return ;
	while (m && m[0] && m[0][i])
	{
		if (m[0][i])
		{
			free(m[0][i]);
			m[0][i] = NULL;
		}
		i++;
	}
	if (m && m[0])
	{
		free(m[0]);
		*m = NULL;
	}
}

/* Frees up to four strings passed as parameters.
Each string is optional and will be freed only if not NULL. */
void	free_all(char *s1, char *s2, char *s3, char *s4)
{
	if (s1 != NULL)
		free(s1);
	if (s2 != NULL)
		free(s2);
	if (s3 != NULL)
		free(s3);
	if (s4 != NULL)
		free(s4);
}

/* Frees temporary data structures used during parsing failure.
Clears the command list and both string arrays.
Returns NULL to allow inline usage on error. */
t_list	*free_tmp_lst(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_lst);
	free_array(&temp);
	free_array(&args);
	return (NULL);
}
