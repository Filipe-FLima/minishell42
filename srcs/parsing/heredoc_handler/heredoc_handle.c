/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:57:30 by flima             #+#    #+#             */
/*   Updated: 2025/04/24 16:26:05 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

static t_pars_err	read_heredoc_input(t_main_data *data, char *file_name, \
	char *delimiter, t_token *current)
{
	int		exit_status;
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid == 0)
		heredoc_reading(data, file_name, delimiter, current);
	waitpid(child_pid, &exit_status, 0);
	if (WEXITSTATUS(exit_status))
	{
		if (set_exit_env_status(data->env_vars, EXIT_FAIL) != SUCCESS)
			return (ERROR_MEM_ALLOC);
		return (SUCCESS);
	}
	else if (WIFSIGNALED(exit_status))
	{
		if (set_exit_env_status(data->env_vars, EXIT_SIGINT) != SUCCESS)
			return (ERROR_MEM_ALLOC);
		g_last_signal = EXIT_SIGINT;
		return (HEREDOC_CHILD_SIGNALED);
	}
	return (SUCCESS);
}

static t_pars_err	creat_file_name(char **file_name, int nbr_heredoc)
{
	char	*prefix;
	char	*heredoc_name;

	prefix = "/tmp/heredoc_";
	heredoc_name = ft_itoa(nbr_heredoc);
	if (heredoc_name == NULL)
		return (ERROR_MEM_ALLOC);
	*file_name = ft_strjoin(prefix, heredoc_name);
	if (file_name == NULL)
	{
		free(heredoc_name);
		return (ERROR_MEM_ALLOC);
	}
	free(heredoc_name);
	return (SUCCESS);
}

static t_pars_err	assign_fd_token(t_token *current, int fd)
{
	free(current->next->value);
	current->next->value = NULL;
	current->next->value = ft_itoa(fd);
	if (current->next->value == NULL)
		return (ERROR_MEM_ALLOC);
	return (SUCCESS);
}

static t_pars_err	get_current_heredoc(t_main_data *data, \
	t_token *current, int *nbr_heredoc)
{
	int				fd;
	t_pars_err		status;
	char			*file_name;
	char			*delimiter;

	status = creat_file_name(&file_name, *nbr_heredoc);
	if (status != SUCCESS)
		return (status);
	(*nbr_heredoc)++;
	delimiter = current->next->value;
	status = read_heredoc_input(data, file_name, delimiter, current);
	if (status != SUCCESS)
	{
		unlink(file_name);
		free(file_name);
		return (status);
	}
	fd = open(file_name, O_RDONLY);
	unlink(file_name);
	free(file_name);
	status = assign_fd_token(current, fd);
	if (status != SUCCESS)
		return (ERROR_MEM_ALLOC);
	return (SUCCESS);
}

t_pars_err	capture_heredocs(t_main_data *data)
{
	int			nbr_of_heredoc;
	t_pars_err	status;
	t_token		*current;

	nbr_of_heredoc = 0;
	current = data->tokens;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			status = get_current_heredoc(data, current, &nbr_of_heredoc);
			if (status != SUCCESS)
			{
				if (status == HEREDOC_CHILD_SIGNALED)
					close_fd_heredoc(data->tokens, current);
				return (status);
			}
		}
		current = current->next;
	}
	return (SUCCESS);
}
