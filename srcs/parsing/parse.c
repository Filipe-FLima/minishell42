/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:29:45 by filipe            #+#    #+#             */
/*   Updated: 2025/03/28 19:06:09 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

void	parser(t_main_data *data)
{
	t_pars_err	status;

	if (data->pipeline == NULL)
		return ;
	if (*(data->pipeline) == '\0')
		return ;
	status = tokenize_input(data, data->pipeline);
	if (status != SUCCESS)
		return (status_error(data, status));
	status = syntax(data);
	if (status != SUCCESS)
		return (status_error_syntax(data, status));
	status = merge_tokens_n_rm_blank_tokens(data);
	if (status != SUCCESS)
		return (status_error(data, status));
	debugging(data);
	status = capture_heredocs(data);
	if (status != SUCCESS)
		return (status_error(data, status));
	debugging(data);
}
