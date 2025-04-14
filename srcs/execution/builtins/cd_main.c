/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:41:37 by yulpark           #+#    #+#             */
/*   Updated: 2025/04/14 18:44:36 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	change_pwd(t_env_var *envp, char *tb_oldpwd, char *home)
{
	t_env_var	*temp;

	temp = envp;
	while (temp)
	{
		if (ft_strncmp(temp->variable, "OLDPWD", 6) == 0)
		{
			free(temp->variable);
			temp->variable = ft_strdup(tb_oldpwd);
		}
		if (ft_strncmp(temp->variable, "PWD", 3) == 0)
		{
			free (temp->variable);
			temp->variable = ft_strdup(home);
		}
		temp = temp->next;
	}
	free(tb_oldpwd);
}

static int	go_home(t_env_var *envp, char* oldpwd)
{
	t_env_var	*home;
	
	home = ft_find_env(envp, "HOME");
	if (!home || chdir(home->variable) != 0)
		return (1);
	change_pwd(envp, oldpwd, home->variable);
	return (0);	
}

static int go_prev(t_env_var *envp, char *tb_old_pwd)
{
	t_env_var	*tb_new_pwd;

	tb_new_pwd = ft_find_env(envp, "OLDPWD");
	if (!tb_new_pwd || chdir(tb_new_pwd->variable) != 0)
		return (1);
	free(tb_old_pwd);
	return (0);
}

static int cases(char *cmd, t_env_var *envp, char *tb_old_pwd)
{
	char	*new_pwd;

	if (access(cmd, F_OK) == 0)
	{
		if (chdir(cmd) != 0)
			return (1);
		new_pwd = getcwd(NULL, 0);
		change_pwd(envp, tb_old_pwd, new_pwd);
		free(new_pwd);
		return (0);
	}
	return (2);
}

t_exec_error	ft_cd(char **cmd, t_env_var *envp)
{
	char	*tb_old_pwd;
	int		res;
	
	if (cmd[2])
	{
		error_msg("minishell: cd: Too many arguments\n");
		return (INVALID_INPUT);
	}
	tb_old_pwd = getcwd(NULL, 0);
	if (cmd[1] == NULL || cmd[1][0] == '~')
	{
		res = go_home(envp, tb_old_pwd);
		if (res == 1)
		{
			free(tb_old_pwd);
			error_msg("minishell: cd: HOME doesn't exist\n");
			return (ENV_ERROR);
		}
		return (SUCCEED);
	}
	if (cmd[1][0] == '-')
	{
		res = go_prev(envp, tb_old_pwd);
		if (res != 0)
		{
			free(tb_old_pwd);
			error_msg("minishell: cd: OLDPWD doesn't exist\n");
			return (ENV_ERROR);
		}
		return (SUCCEED);
	}
	res = cases(cmd[1], envp, tb_old_pwd);
	if (res == 1)
	{
		error_msg("minishell: cd: Path doesn't exist\n");
		return (ENV_ERROR);
	}
	if (res == 2)
	{
		error_msg("minishell: cd: No access to the path\n");
		return (ENV_ERROR);
	}
	return (SUCCEED);
}
