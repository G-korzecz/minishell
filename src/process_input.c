/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:39:27 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:39:30 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Helper for freeing input whn encounter unclosed quotes. */
static void	handle_uquote(char **s, t_cmd_set *p)
{
	put_err("Unclosed_Quote", NULL, 2, p);
	free_all(*s, NULL, NULL, NULL);
}

/* Checks if a string has unclosed single or double quotes
that are not respectivly themselves into double/single quotes
Returns 1 if unclosed, 0 otherwise. */
int	check_unclosed_quotes(const char *s)
{
	int	squote;
	int	dquote;
	int	i;

	squote = 0;
	dquote = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' && dquote == 0)
			squote = !squote;
		else if (s[i] == '\"' && squote == 0)
			dquote = !dquote;
		i++;
	}
	return (squote || dquote);
}

/* Processes the user input: validates quotes, tokenizes input,
parses commands, and prepares them for execution. 
Also don't forget to add input to history for arry manipulation
via readline.*/
void	*process_input(char *input, t_cmd_set *p)
{
	char	**a;
	int		i[3];
	int		quotes[2];

	i[0] = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	if (!input)
		return (NULL);
	if (input[0] != '\0')
		add_history(input);
	handle_input(&input, i, quotes, p);
	if (check_unclosed_quotes(input))
	{
		handle_uquote(&input, p);
		return (p);
	}
	a = split_and_ignore_space_if_in_quote(input, " ");
	if (!a)
		free_exit(p, 1, "Unexpected Error");
	p = parse_nodes(a, p);
	free_all(input, NULL, NULL, NULL);
	if (p && p->cmds)
		ft_lstclear(&p->cmds, free_lst);
	return (p);
}
