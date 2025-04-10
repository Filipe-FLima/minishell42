/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:26:53 by yulpark           #+#    #+#             */
/*   Updated: 2025/04/09 16:01:21 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#include <dirent.h>
#include "minishell.h"
#include "libft.h"
#include "tokenization.h"

typedef enum s_exec_error{
	SUCCEED,
	MALLOC_FAIL,
	NO_INPUT,
	INVALID_INPUT,
	FAIL_PATH_RETRIEVAL,
	ENV_ERROR
}	t_exec_error;

int				distribution(t_main_data *data);

//builtins

//t_exec_error	ft_cd(char **cmd, t_env_var **envp);
int				echo(char **args);
int				env(t_env_var *envp, char **args);
void			ft_exit(char **cmd, t_main_data *data);
int				export_arg(char *arg, t_env_var **envp);
int				export(char **args, t_env_var **envp);
t_exec_error	pwd();
int				ft_unset(char **args, t_env_var **envp);

//export_utils
char			*get_var_name(char *arg);
int				input_checker(char *input);
int				create_new_var(char *arg, t_env_var **envp);
int				ft_add_key_val(t_env_var **head, char *keyvalue);

// cd_utils
t_exec_error	input_counter(char **cmd);
char			*ft_strputjoin(char *src1, char *src2, char c);
char			*update_env_value(char *s1, char *s2);

//cd_cases
t_exec_error	go_home(t_env_var **envp, t_env_var *PWD);
t_exec_error	go_prev(t_env_var **envp,  t_env_var *PWD, t_env_var *OLDPWD);
t_exec_error	relative_path(char *cmd, t_env_var **envp, t_env_var *PWD, t_env_var *OLDPWD);
t_exec_error	absolute_path(char *cmd, t_env_var **envp, t_env_var *PWD, t_env_var *OLDPWD);

//env_utils
t_env_var		*ft_find_env(t_env_var *env, char *key);
int				ft_env_update(t_env_var *env, char *name, char *path);
void			ft_delete_node(t_env_var **envp, t_env_var *head, int i); // used in unset
#endif


