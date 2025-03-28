/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:26:53 by yulpark           #+#    #+#             */
/*   Updated: 2025/03/20 18:22:53 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#include <dirent.h>
#include "../../includes/minishell.h"
#include "../../includes/tokenization.h"

typedef struct s_address
{
	char *address;
	struct s_address *prev;
	struct s_address *next;
}	t_address;

typedef struct s_loc{
	char *old_pwd;
	char *pwd;
	char *env;
}	t_loc;

typedef struct s_varlist
{
	char *key;
	char *content;
	struct s_varlist *prev;
	struct s_varlist *next;
}	t_varlist;


//t_env *ft_find_env(t_env *env, char *key);
//int ft_env_update(t_env *env, char *name, char *path);
//char	*ft_strputjoin(char *src1, char *src2, char c, char *dst);
//int	relative(char **args, t_env *env);
//int	cd(char **args , t_env *env);
#endif


