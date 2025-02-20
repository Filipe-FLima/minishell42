/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filipe <filipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:49:35 by filipe            #+#    #+#             */
/*   Updated: 2025/02/20 22:14:28 by filipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strtoken(char *str)
{
    int     i;
    int     start;
    char    *token;

    while (str[i] == '\t' || str[i] == '\n' || \
        str[i] == '\r' || str[i] == ' ')
        i++;
    if (!str[i])
        return (NULL);
    start = i;
    while (str[i] != '\t' && str[i] != '\n' && \
        str[i] != '\r' && str[i] != ' ' && !str[i])
        i++;
    token = ft_substr(str, start, i - start);
    return (token);
}