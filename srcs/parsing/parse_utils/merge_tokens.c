/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:45:28 by flima             #+#    #+#             */
/*   Updated: 2025/04/28 20:01:24 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

static void	remove_blank_tokens(t_token **current)
{
	t_token	*tmp;

	while (*current != NULL)
	{
		if ((*current)->type == NEW_LINE || \
			(*current)->type == TAB_CHAR || \
			(*current)->type == SPACE_CHAR)
		{
			tmp = *current;
			*current = (*current)->next;
			tmp->next = NULL;
			free_tokens(tmp);
		}
		else
			current = &(*current)->next;
	}
}

static t_pars_err	merge_quotes(t_token *current)
{
	char	*tmp_str;

	while (current->next && (current->next->type == D_QUOTE || \
			current->next->type == S_QUOTE || \
			current->next->type == WORD || current->next->type == VARIABLE))
	{
		tmp_str = current->value;
		if (current->type == VARIABLE && ft_strlen(current->value) == 1 \
		&& current->value[0] == '$' && (current->next->type == D_QUOTE || \
		current->next->type == S_QUOTE))
			current->value = ft_strdup(current->next->value);
		else
			current->value = ft_strjoin(tmp_str, current->next->value);
		if (current->value == NULL && errno == ENOMEM)
			return (ERROR_MEM_ALLOC);
		free(tmp_str);
		remove_next_token(current);
	}
	return (SUCCESS);
}

static t_pars_err	merge_env_var(t_token *current)
{
	char	*tmp_str;

	if (current->next && current->next->type == WORD)
	{
		tmp_str = current->value;
		current->value = ft_strjoin(tmp_str, current->next->value);
		if (current == NULL && errno == ENOMEM)
			return (ERROR_MEM_ALLOC);
		free(tmp_str);
		remove_next_token(current);
	}
	return (SUCCESS);
}

t_pars_err	merge_variable(t_main_data *data)
{
	t_token	*current;

	current = data->tokens;
	while (current != NULL)
	{
		if (current->type == VARIABLE)
		{
			if (merge_env_var(current) != SUCCESS)
				return (ERROR_MEM_ALLOC);
		}
		current = current->next;
	}
	return (SUCCESS);
}

t_pars_err	merge_tokens_n_rm_blank_tokens(t_main_data *data)
{
	t_token	*current;

	current = data->tokens;
	if (merge_variable(data) != SUCCESS)
		return (ERROR_MEM_ALLOC);
	if (expand_token_n_trim_quote(data->env_vars, current) != 0)
		return (ERROR_MEM_ALLOC);
	while (current != NULL)
	{
		if (current->type == D_QUOTE || \
			current->type == S_QUOTE || \
			current->type == WORD || \
			current->type == VARIABLE)
		{
			if (merge_quotes(current) != SUCCESS)
				return (ERROR_MEM_ALLOC);
		}
		current = current->next;
	}
	remove_blank_tokens(&data->tokens);
	return (SUCCESS);
}
