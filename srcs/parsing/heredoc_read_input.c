/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read_input.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:13:08 by flima             #+#    #+#             */
/*   Updated: 2025/04/11 20:21:35 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

void	hered_err_exit(t_main_data *data, t_exit_code status, char *msg)
{
	if (status == EXIT_FAIL)
	{
		error_msg("child process exit failure\n");
		clean_all_data_exit(data, EXIT_FAIL); //rever esse comportamento 
	}
	else if (status == EXIT_DENIED)
	{
		error_msg("permission denied or no such file or directory: ");
		write (STDOUT_FILENO, msg, ft_strlen(msg));
		write (STDOUT_FILENO, "\n", 1);
		free(msg);
		clean_all_data_exit(data, EXIT_FAIL);
	}
}


static t_exit_code	readline_heredoc(t_main_data *data, int fd, char *delim, \
		t_syntax type)
{
	char	*input;

	while (true)
	{
		input = readline(">");
		if (input == NULL && errno == ENOMEM)
			return (EXIT_MEM_FAILURE);
		if (input == NULL)
		{
			error_msg("warning: here-document delimited by end-of-file\n");
			break ;
		}
		if (!ft_strncmp(input, delim, ft_strlen(delim) + 1))
		{
			free(input);
			break ;
		}
		if (type == WORD)
			if (expand_env_instr(data->env_vars, &input) == ERROR_MEM_ALLOC)
				return (EXIT_MEM_FAILURE);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	return (EXIT_SUCCESSFULLY);
}

void	heredoc_reading(t_main_data *data, char *file_name, \
	char *delim, t_token *current)
{
	int			fd;
	t_exit_code	status;

	setup_signal_handlers(HEREDOC_CHILD);
	rl_clear_history();
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(file_name);
		hered_err_exit(data, EXIT_FAIL, NULL);
	}
	status = readline_heredoc(data, fd, delim, current->next->type);
	close(fd);
	if (status != EXIT_SUCCESSFULLY)
	{
		if (status == EXIT_MEM_FAILURE)
			hered_err_exit(data, EXIT_FAIL, NULL);
		else
			hered_err_exit(data, EXIT_DENIED, file_name);
	}
	free(file_name);
	clean_all_data_exit(data, EXIT_SUCCESSFULLY);
}
