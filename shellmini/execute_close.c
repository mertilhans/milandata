/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_close.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:53:42 by husarpka          #+#    #+#             */
/*   Updated: 2025/08/19 22:11:45 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	close_pipe_fds(t_exec_data *data)
{
	if (data->pipefd[0] > 2)
	{
		close(data->pipefd[0]);
		data->pipefd[0] = -1;
	}
	if (data->pipefd[1] > 2)
	{
		close(data->pipefd[1]);
		data->pipefd[1] = -1;
	}
}

void	ft_free_and_close(t_exec_data *data, t_parser *cmd)
{
	if (data->original_stdin > 2)
	{
		close(data->original_stdin);
		data->original_stdin = -1;
	}
	if (data->original_stdout > 2)
	{
		close(data->original_stdout);
		data->original_stdout = -1;
	}
	close_pipe_fds(data);
	if (data->in_fd != STDIN_FILENO && data->in_fd > 2)
	{
		close(data->in_fd);
		data->in_fd = STDIN_FILENO;
	}
	close_all_heredoc_fds(cmd);
	gb_free_all();
	env_gb_free_all();
}

void	close_all_heredoc_fds(t_parser *cmd_list)
{
	t_parser	*current;

	current = cmd_list;
	while (current)
	{
		if (current->heredoc_fd > 2)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = current->next;
	}
}
