/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:29:16 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/15 15:29:23 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*void print_current_export(char **envp)
{
    int i;
    int j;
    char *after_equal;

    i = 0;
    while (envp[i] != NULL)
    {
        j = 0;
        printf("declare -x ");
        while (envp[i][j] && envp[i][j] != '=')
        {
            printf("%c", envp[i][j]);
            j++;
        }
        if (ft_strchr(envp[i], '='))
        {
            after_equal = ft_strchr(envp[i], '=') + 1;
            printf("=\"%s\"", after_equal);
        }
        else
            printf("\n");
        i++;
    }
}

int check_export_format(char *cmd_args)
{
    int i;

    i = 0;
    if (!cmd_args || !ft_isalpha(cmd_args[i]) && cmd_args[i] != '_')  // The first character must be a letter or an underscore
        return (0);
    while (cmd_args[i] && cmd_args[i] != '=')        // If there's an '=', check only the variable name before the equal sign
    {
        if (!ft_isalnum(cmd_args[i]) && cmd_args[i] != '_')  // Other positions must be letters, digits, or underscores
            return (0);
        i++;
    }
    return (1);
}

int check_exist(char *var_name, char **envp)   // If the variable exists, return the index of the environment variable
{
    int i;

    i = 0;
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], var_name, ft_strlen(var_name)) && envp[i][ft_strlen(var_name)] == '=')  // Check if there's the variable name + '=' in envp
            return (i);
        i++;
    }
    return (-1);
}

int append_envp(char **envp, char *var_name, char *value)
{
    int envp_size;

    envp_size = 0;
    while (envp[envp_size] != NULL)
        envp_size++;
    if (envp_size < MAX_VARS)   //!!!!!!!!! Define MAX_VARS
    {
        if (value)
            envp[envp_size] = ft_strjoin(ft_strjoin(var_name, '='), value);
        else
            envp[envp_size] = ft_strdup(var_name);
        if (!envp[envp_size] )
            return (-1);
    }
    else
    {
        printf("Error: Maximum environment variables reached.\n");
        exit(-1);
    }
    return (0);
}

int update_envp(char *envp2update, char *var_name, char *value)
{
    free(envp2update);
    if (value)
        envp2update = ft_strjoin(ft_strjoin(var_name, '='), value);
    else  // If there is only the variable name
        envp2update = ft_strdup(var_name);
    if (!envp2update)
        return (-1);
    return (0);
}

int export_var(char *cmd_args, char **envp)
{
    char *value;
    char *var_name;
    char *equal_pos;
    int idx_exist;

    var_name = ft_strdup(cmd_args);
    value = NULL;
    equal_pos = ft_strchr(var_name, '=');
    if (equal_pos)
    {
        *equal_pos = '\0';      // Separate variable name and value
        value = equal_pos + 1;
    }
    idx_exist = check_exist(var_name, envp);    
    if ((idx_exist >= 0 && update_envp(envp[idx_exist], var_name, value) == -1) ||
        (idx_exist < 0 && append_envp(envp, var_name, value) == -1))
    {
        free(var_name);
        return (0);
    }
    free(var_name);
    return (1);
}


int builtin_export(char **cmd_args, char **envp)
{
    int i;
    int exit_code;

    i = 1;
    exit_code = 0;
    if (!cmd_args[i])      // If only "export" is provided, print all currently exported environment variables
    {
        print_current_export(envp);
        return (exit_code);
    }
    while (cmd_args[i] != NULL)
    {
        if (!check_export_format(cmd_args[i]))
        {
            printf("Export: invalid identifier\n");
            exit_code = 1;
        }
        else if (export_var(cmd_args[i], envp) == 0)    // If export fails
        {
            printf("Memory allocation failed\n");
            exit_code = 1;
        }
        i++;
    }
    return (exit_code);
}*/

int builtin_export(void)
{
    printf("testexport\n");
    return (255);
}