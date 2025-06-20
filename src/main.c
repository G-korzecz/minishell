/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:37:08 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:37:24 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Generate the current working directory via getcwd, print it in bold.
If getcwd fail, put root instead "/" so the program can continue.
Append "$ " at the end. */
static char	*generate_prompt(void)
{
	char	*cwd;
	char	*colorcwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		cwd = ft_strdup("/");
	if (cwd == NULL)
		return (NULL);
	colorcwd = ft_strjoin("\001\033[1;34m\002", cwd);
	if (colorcwd == NULL)
		return (free(cwd), NULL);
	prompt = ft_strjoin(colorcwd, "\001\033[0m\002$ ");
	if (prompt == NULL)
	{
		free_all(cwd, colorcwd, NULL, NULL);
		return (NULL);
	}
	free_all(cwd, colorcwd, NULL, NULL);
	return (prompt);
}

/* Normal case for Minishell :
use readline to get user input (readline handle arrow keys, history, tab...)
prompt is current working directory -> p->input_text.*/
static void	handle_interactive_input(t_cmd_set *p)
{
	char	*prompt;

	prompt = generate_prompt();
	if (prompt == NULL)
		free_exit(p, 1, "Prompt error");
	p->input_text = readline(prompt);
	free(prompt);
	if (p->input_text == NULL)
	{
		ft_putendl_fd("\033[1;31mexit\033[0m", 1);
		if (g_exit_status < 0)
			g_exit_status = 0;
		free_exit(p, g_exit_status, NULL);
	}
}

/* Test case for minishell, use get next line on stdin to get
a list of instruction from a file.
Example : "./minishell < test_script.sh"
Remove next line delimiter "\n" for processing. */
static void	handle_non_interactive_input(t_cmd_set *p)
{
	char	*line;

	p->input_text = get_next_line(0);
	if (!p->input_text)
		free_exit(p, p->status_code, NULL);
	line = p->input_text;
	p->input_text = ft_strtrim(p->input_text, "\n");
	if (p->input_text == NULL)
		free_exit(p, g_exit_status, "exit\n");
	free_all(line, NULL, NULL, NULL);
}

/* Init & set_signal initialise everything we use.
isatty(0) = Is standard input connected to a terminal?
if yes, interactive mode, if not, non-interactive mode. */
int	main(int argc, char **argv, char **envp)
{
	t_cmd_set	p_cmd_set;

	init(&p_cmd_set, envp, argv, argc);
	while (true)
	{
		disable_ctrl_z();
		set_signals(&p_cmd_set);
		if (isatty(0))
			handle_interactive_input(&p_cmd_set);
		else
			handle_non_interactive_input(&p_cmd_set);
		if (!process_input(p_cmd_set.input_text, &p_cmd_set))
			break ;
	}
	free_exit(&p_cmd_set, p_cmd_set.status_code, NULL);
}
