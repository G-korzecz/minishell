/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:28:37 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/15 15:29:04 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*int check_unset_format(char *cmd_args)
{
    int i;

    i = 0;
    if (!cmd_args || !ft_isalpha(cmd_args[i]) && cmd_args[i] != '_')  // The first character must be a letter or an underscore
        return (0);
    while (cmd_args[i] != '\0')
    {
        if (!ft_isalnum(cmd_args[i]) && cmd_args[i] != '_')  // Other positions must be letters, digits, or underscores
            return (0);
        i++;
    }
    return (1);
}

int unset_var(char *var_name, char **envp)
{
    int idx_exist;

    idx_exist = check_exist(var_name, envp);    
    if (idx_exist >= 0)  // If the variable exists, delete
    {
        free(envp[idx_exist]);
        i = idx_exist;
        while (envp[i] != NULL)
        {
            envp[i] = envp[i + 1];
            i++;
        }
        return (1);
    }
    else
    {
        printf("Variable %s not found in environment\n", var_name);  //not an error
        return (-1);
    }
    return (0);   //fail unset
}

int builtin_unset(char **cmd_args, char **envp)
{
    int i;
    int exit_code;

    i = 1;
    exit_code = 0;
    if (!cmd_args[i])
        return (0);
    while (cmd_args[i] != NULL)
    {
        if (!check_unset_format(cmd_args[i]))
        {
            printf("Export: invalid identifier\n");
            exit_code = 1;
        }
        else if (unset_var(cmd_args[i], envp) == 0)    // If unset fails
        {
            printf("Error: Unable to unset variable %s\n", cmd_args[i]);
            exit_code = 1;
        }
        i++;
    }
    return (exit_code);
}*/

int builtin_unset(void)
{
    return (255);
}