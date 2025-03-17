/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:03:24 by flima             #+#    #+#             */
/*   Updated: 2025/03/17 18:55:14 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

// identify syntax erros in the tokens list
t_token	*skip_blank_nodes_n_get_next(t_token *tokens, int flag)
{
	t_token	*current;

	if (tokens == NULL)
		return (NULL);
	if  (flag != 0)
		current = tokens;
	else
		current = tokens->next;
	while (current && (current->type == SPACE_CHAR || \
		current->type == NEW_LINE || current->type == TAB_CHAR))
		current = current->next;
	return (current);
}

static void	assign_syntax_func(t_syntax_check *table)
{
	table[0] = NULL;
	table[1] = NULL;
	table[2] = NULL;
	table[3] = syntax_pipe;
	table[4] = NULL;
	table[5] = NULL;
	table[6] = NULL;
	table[7] = NULL;
	table[8] = NULL;
	table[9] = NULL;
	table[10] = NULL;
	table[11] = NULL;
}
t_pars_err	syntax(t_main_data *data)
{
	t_token			*current;
	t_token			*previous;
	t_pars_err		status;
	t_syntax_check	syntax_table[12];

	
	previous = NULL;
	current = data->tokens;
	assign_syntax_func(syntax_table);
	current = skip_blank_nodes_n_get_next(current, 1);
	while (current != NULL)
	{
		if (syntax_table[current->type] != NULL)
			status = syntax_table[current->type](previous, current);
		else
			return(SUCCESS); //for test, remove after
		if (status != SUCCESS)
			return (status);
		previous = current;
		current = skip_blank_nodes_n_get_next(current, 0);
	}
	return (SUCCESS);
}