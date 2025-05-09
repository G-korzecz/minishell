/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:14:49 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/29 17:15:16 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Finds the index of a variable named "var" in the envp array.
Compares only the name part before '='.
Returns the index if found, or -1 if not found. */
int	find_env_var_index(char *var, char **envp)
{
	int	i;
	int	n;
	int	n2;

	i = 0;
	n = ft_strlen(var);
	while (envp && envp[i])
	{
		n2 = n;
		if (n2 < ft_strchr_idx(envp[i], '='))
			n2 = ft_strchr_idx(envp[i], '=');
		if (!ft_strncmp(envp[i], var, n2))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

/* Retrieves the value of an environment variable named "var".
Returns a newly allocated string containing the value.
If the variable does not exist, returns NULL. */
char	*ft_getenv(char *var, char **envp)
{
	int			var_index;
	const char	*env_var;
	const char	*value_start;

	if (var[0] == 0)
		return (ft_strdup("'$'"));
	var_index = find_env_var_index(var, envp);
	if (var_index == -1)
		return (NULL);
	env_var = envp[var_index];
	value_start = ft_strchr(env_var, '=');
	if (value_start)
	{
		value_start++;
		return (ft_strdup(value_start));
	}
	return (NULL);
}

/* Sets or updates an environment variable in envp.
If the variable exists, its value is replaced.
If not, a new entry is added to the array. */
char	**ft_setenv(char *var, char *value, char **envp)
{
	int		index;
	char	*tmpstr[2];

	index = find_env_var_index(var, envp);
	if (index != -1)
	{
		tmpstr[0] = ft_strjoin(var, "=");
		if (tmpstr[0])
		{
			free(envp[index]);
			envp[index] = ft_strjoin(tmpstr[0], value);
			free(tmpstr[0]);
		}
	}
	else
	{
		tmpstr[0] = ft_strjoin(var, "=");
		if (tmpstr[0])
		{
			tmpstr[1] = ft_strjoin(tmpstr[0], value);
			envp = ft_array_insert(envp, tmpstr[1]);
			free_all(tmpstr[0], tmpstr[1], NULL, NULL);
		}
	}
	return (envp);
}

/* Expands an environment variable in a string.
Extracts the variable name, retrieves its value,
and writes it to the pipe output. */
void	handle_env_vars(char *str, int *i, int fd[2], t_cmd_set *p)
{
	int		j;
	char	*tmp;
	char	*tmp2;

	j = *i + 1;
	while (str[j])
	{
		if (!ft_isalnum(str[j]) || str[j] == ' ' || str[j] == '\n'
			|| str[j] == '\t' || str[j] == '$' || str[j] == '\"'
			|| str[j] == '\'' || str[j] == '<')
			break ;
		j++;
	}
	tmp = ft_substr(str, *i + 1, j - *i - 1);
	tmp2 = ft_getenv(tmp, p->envp);
	write(fd[1], tmp2, ft_strlen(tmp2));
	free_all(tmp, tmp2, NULL, NULL);
	*i = j - 1;
}
