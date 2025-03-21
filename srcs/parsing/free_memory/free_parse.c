/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:17:50 by flima             #+#    #+#             */
/*   Updated: 2025/03/17 19:32:17 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

void	free_tokens(t_token **tokens)
{
	t_token	*tmp;

	while (*tokens != NULL)
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->value);
		free(tmp);
	}
}

void	clean_all_data(t_main_data *data)
{
	free_tokens(&data->tokens);
	free(data->pipeline);
}

void	clean_all_data_error(t_main_data *data, int out_status)
{
	rl_clear_history();
	free_tokens(&data->tokens);
	free(data->pipeline);
	exit(out_status);
}
