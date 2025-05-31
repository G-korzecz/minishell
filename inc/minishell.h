/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:19:14 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/17 18:19:17 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"
# include "../ft_printf_fd/ft_printf_fd.h"
# include <dirent.h>            
# include <fcntl.h>             
# include <signal.h>            
# include <stdio.h>             
# include <sys/ioctl.h>         
# include <sys/wait.h>          
# include <readline/readline.h>
# include <readline/history.h> 
# include <stdarg.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/stat.h>	

/* Used to store the exit status of last command used
Extern so any function can use it (global variable)
Use volatile to ensure that even if the variable is modified
by signals i must not be optimized by the program
sig_atomic_t : basically modified in only ONE operation.*/
extern volatile sig_atomic_t	g_exit_status;

/* Struct which are saved in the content of a linked list for each command
args : [0] : cmd_name, [1]...[end] = NULL arguments as per execve execution.
path for exeve
in fd/out fd > redirection, piping...*/
typedef struct s_cmd
{
	char	**args;
	char	*cmd_path;
	int		in_fd;
	int		out_fd;
}			t_cmd;

/* Main structure, hold the chaine list of command
all env variables
the input text
status_code
pid of the last command.*/
typedef struct s_cmd_set
{
	t_list	*cmds;
	char	**envp;
	char	*input_text;
	int		status_code;
	pid_t	pid_of_lst_cmd;
}			t_cmd_set;

/* Error Handling */

void	*put_err(char *err_type, char *param, int err, t_cmd_set *p);
void	*put_err_cd(char *err_type, char *path, int code, t_cmd_set *p);
void	*put_err_syntax(char *err_msg, int err_code, t_cmd_set *p);
void	error_token_newline(void);
void	error_unexpected_token(char *cmd);
void	error_unclosed_quotes(void);
void	error_delim_heredoc(char *lim);

/* Signal Handling */

void	signals_parent(int signal_code);
void	signals_child(int signal_code);
void	signals_heredoc(int signal_code);
void	set_signals(t_cmd_set *p);
void	disable_ctrl_z(void);

/* String and Array Manipulation */

int		ft_arr_len(char **m);
int		chrset_idx(const char *s, char *set);
int		char_index(const char *s, int c);
char	**ft_dup_array(char **m);
char	**ft_array_insert(char **in, char *newstr);
char	**ft_array_replace(char ***big, char **small, int n);
char	**split_and_ignore_space_if_in_quote(char *s, char *set);
char	*remove_quotes(char const *s1, int squote, int dquote);

/* Environement manipulation */

char	*ft_getenv(char *var, char **envp);
char	**ft_setenv(char *var, char *value, char **envp);
int		find_env_var_index(char *var, char **envp);

/* Expand Variables*/

char	*var_expander(char *str, int quotes[2], t_cmd_set *p);
char	*expand_variable(char *str, int i, t_cmd_set *p, char *s[3]);
char	*remove_dollar_quote(char *str);
char	*find_substitution(char first, char *var, t_cmd_set *p);
void	track_quotes(int *in_squote, int *in_dquote, char c);
char	*remove_char_at(char *str, size_t pos);
void	remove_curly_brackets(char **s, int i[3], int quotes[2], char *tmp[3]);

/* Free and Exit */

void	free_exit(t_cmd_set *p, int exit_code, char *msg);
void	free_lst(void *content);
void	free_all(char *s1, char *s2, char *s3, char *s4);
void	free_array(char ***m);
t_list	*free_tmp_lst(t_list *cmds, char **args, char **temp);
void	close_fds_except_std(void);

/* Initialisation */

void	init(t_cmd_set *p, char **envp, char **argv, int argc);

/* Command Processing and Execution */

int		check_unclosed_quotes(const char *s);
void	*process_input(char *out, t_cmd_set *p);
void	process_heredoc(char **s, int i[3], int quotes[2], char *tmp[3]);
void	handle_input(char **input, int i[3], int quotes[2], t_cmd_set *p);
int		update_quotes_chk_heredoc(int *quo, char ch, int i[3], char **s);
void	find_cmd_path(t_cmd_set *p, t_list *cmd, char **s, char *path);
char	*ensure_path(char *path);
void	handle_exec_err(t_cmd *n, DIR *dir, t_cmd_set *p);
void	*chk_perm_call_child(t_cmd_set *p, t_list *cmd, int fd[2]);
void	*parse_nodes(char **args, t_cmd_set *p);
char	**split_with_special_chars(char **args);
t_cmd	*out_fd_truncate(t_cmd *node, char **args, int *i, t_cmd_set *p);
t_cmd	*out_fd_append(t_cmd *node, char **args, int *i, t_cmd_set *p);
t_cmd	*in_fd_read(t_cmd *node, char **args, int *i, t_cmd_set *p);
t_cmd	*in_fd_heredoc(t_cmd *node, char **args, int *i, t_cmd_set *p);
void	handle_env_vars(char *str, int *i, int fd[2], t_cmd_set *p);
void	exec_cmd_and_wait(t_cmd_set *p, int status, int tmp[2]);
t_cmd	*init_cmd(void);
char	*is_invalid_syntax(char **tok, int *i);
int		exec_par_builtins(t_cmd_set *p, t_list *cmd, int n);
void	*setup_pipe_and_fork_cmd(t_cmd_set *p, t_list *cmd);
void	exec_child_builtins(t_cmd_set *p, t_cmd *n, int l, t_list *cmd);

/* Builtins */

int		is_builtin(t_cmd *n);
void	builtin_exit(t_list *cmd, t_cmd_set *p);
int		builtin_exit_child(t_list *cmd);
int		builtin_env(char **m);
int		builtin_export(t_cmd_set *p, char **args);
int		builtin_unset(t_cmd_set *p, char **args);
int		builtin_pwd(void);
int		builtin_echo(t_list *cmd);
int		builtin_cd(t_cmd_set *p, char **cmd_args);

#endif
