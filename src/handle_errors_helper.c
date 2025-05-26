/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:06:41 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/25 16:09:02 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Helpers for "line too long." norminette"*/
void	error_unclosed_quotes(void)
{
	ft_printf_fd(2, "mini: ");
	ft_printf_fd(2, "unexpected EOF while looking for matching `'\"\n");
}

void	error_unexpected_token(char *cmd)
{
	ft_printf_fd(2, "mini: ");
	ft_printf_fd(2, "syntax error near unexpected token `%s'\n", cmd);
}

void	error_token_newline(void)
{
	ft_printf_fd(2, "mini: ");
	ft_printf_fd(2, "syntax error near unexpected token `newline'\n");
}
