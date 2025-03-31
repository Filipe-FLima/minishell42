/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:29:38 by flima             #+#    #+#             */
/*   Updated: 2025/03/31 18:03:52 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

t_pars_err get_environ_var_value(char **environ_var, char *var_name)
{
	char *tmp_name;
	char *tmp_var;

	tmp_name = ft_strjoin(var_name, "=");
	if (tmp_name == NULL && errno == ENOMEM)
		return (ERROR_MEM_ALLOC);
	tmp_var = ft_strtrim(*environ_var, tmp_name);
	free(tmp_name);
	if (tmp_var == NULL && errno == ENOMEM)
		return (ERROR_MEM_ALLOC);
	free(*environ_var);
	*environ_var = tmp_var;
	return (SUCCESS);
}

t_pars_err expand_environ_var(t_env_var *envp, char *var_name, char **environ_var)
{
	t_pars_err status;

	// check if var_name is $?
	status = find_environment_var(envp, var_name, environ_var);
	if (status == ERROR_MEM_ALLOC)
		return (ERROR_MEM_ALLOC);
	if (*environ_var[0] == '\0')
		return (SUCCESS);
	status = get_environ_var_value(environ_var, var_name);
	if (status == ERROR_MEM_ALLOC)
		return (ERROR_MEM_ALLOC);
	return (SUCCESS);
}

t_pars_err extract_var_name(char *str, char **var_name)
{
	int len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	if (str[len] == '?' && len == 0)
		len++;
	*var_name = ft_substr(str, 0, len);
	if (*var_name == NULL && errno == ENOMEM)
		return (ERROR_MEM_ALLOC);
	return (SUCCESS);
}

t_pars_err append_expanded_var(char **expand, char *var_value, int *i, \
		char *var_name)
{
	char *new_expand;
	int len;

	len = ft_strlen(*expand) + ft_strlen(var_value) - ft_strlen(var_name);
	new_expand = ft_calloc(len + 1, sizeof(char));
	if (new_expand == NULL && errno == ENOMEM)
		return (ERROR_MEM_ALLOC);
	ft_strlcpy(new_expand, *expand, *i + 1);
	ft_strlcat(new_expand, var_value, len);
	ft_strlcat(new_expand, *expand + (*i + ft_strlen(var_name) + 1), len);
	(*i) += ft_strlen(var_value - 1);
	free(*expand);
	*expand = new_expand;
	return (SUCCESS);
}

t_pars_err	expand_valid_env(t_env_var *envp, char **expand, int *i)
{
	char 		*var_value;
	char 		*var_name;
	t_pars_err	status;
	
	status = extract_var_name(*expand + *i + 1, &var_name);
	if (status == ERROR_MEM_ALLOC)
		return (ERROR_MEM_ALLOC);
	//in case of error, free var_name and var_value
	status = expand_environ_var(envp, var_name, &var_value);
	if (status == ERROR_MEM_ALLOC)
	{
		free(var_name);
		return (ERROR_MEM_ALLOC);
	}
	status = append_expanded_var(expand, var_value, i, var_name);
	if (status == ERROR_MEM_ALLOC)
	{
		free(var_name);
		free(var_value);
		return (ERROR_MEM_ALLOC);
	}
	free(var_name);
	free(var_value);
	return (SUCCESS);
}

t_pars_err expand_env_instr(t_env_var *envp, char **expand)
{
	int 		i;
	t_pars_err	status;

	i = 0;
	while ((*expand)[i])
	{
		if ((*expand)[i] == '$' && (*expand)[i + 1] && (ft_isalnum((*expand)[i + 1])\
			|| (*expand)[i + 1] == '?' || (*expand)[i + 1] == '_'))
		{
			status = expand_valid_env(envp, expand, &i);
			if (status == ERROR_MEM_ALLOC)
				return (ERROR_MEM_ALLOC);
		}
		// else if (expand[i] == '$' && expand[i + 1] != ' ')
		// {
		// 	var_name = extract_var_name(expand + i + 1);
		// 	// if (var_name == NULL)
		// 	// 	return (ERROR_MEM_ALLOC);
		// 	append_expanded_var(&expand, var_name + 1, &i, var_name);
		// 	// if (var_name == NULL)
		// 	// {
		// 	// 	free(var_name);
		// 	// 	return (ERROR_MEM_ALLOC);
		// 	// }
		// }
		else
			i++;
	}
	return (SUCCESS);
}
