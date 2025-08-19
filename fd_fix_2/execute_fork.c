/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 05:49:04 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/19 21:18:54 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

int	setup_and_fork(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	if (cmd->next)
	{
		if (pipe(data->pipefd) == -1)
		{
			perror("pipe");
			return (0);
		}
	}
	else
	{
		data->pipefd[0] = -1;
		data->pipefd[1] = -1;
	}
	fork_pipe_exec(cmd,data,env_list);
	return (0);
}

int	fork_pipe_exec(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (cmd->next)
			close_pipe_fds(data);
		return (0);
	}
	if (pid == 0)
	{
		child_process_exec(cmd, data, env_list);
		exit(1);
	}
	data->pids[data->i++] = pid;
	return (pid);
}

int	setup_pipe_if_needed(t_parser *cmd, t_exec_data *data)
{
	if (!cmd->next)
		return (0);
	if (pipe(data->pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}
