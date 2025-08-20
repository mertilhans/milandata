/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 05:48:53 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/19 22:18:29 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

void	ft_pipe_ctrl(t_parser *cmd, t_exec_data *data)
{
	if (cmd->heredoc_fd > 2)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			close(cmd->heredoc_fd);
			ft_free_and_close(data, cmd);
			exit(1);
		}
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	else if (data->in_fd != STDIN_FILENO)
	{
		if (dup2(data->in_fd, STDIN_FILENO) == -1)
		{
			close(data->in_fd);
			ft_free_and_close(data, cmd);
			exit(1);
		}
		close(data->in_fd);
	}
}

void	redir_and_exec(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	if (cmd->next && data->pipefd[1] > 2)
	{
		if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
		{
			close_pipe_fds(data);
			ft_free_and_close(data, cmd);
			exit(1);
		}
		close(data->pipefd[0]);
		close(data->pipefd[1]);
	}
	if (ft_redir_ctrl(cmd) == -1)
	{
		ft_free_and_close(data, cmd);
		exit(1);
	}
	ft_exec_start(cmd, data, env_list);
}

void	child_process_exec(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	t_parser	*temp;

	setup_child_signals();
	temp = cmd->next;
	while (temp)
	{
		if (temp->heredoc_fd > 2)
		{
			close(temp->heredoc_fd);
			temp->heredoc_fd = -1;
		}
		temp = temp->next;
	}
	ft_pipe_ctrl(cmd, data);
	redir_and_exec(cmd, data, env_list);
}

void	ft_exec_builtin_ctrl(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	int	result;

	if (data->pipefd[0] < 0 && cmd->next)
	{
		ft_free_and_close(data, cmd);
		exit(1);
	}
	result = ft_builtin_call(cmd, data, env_list);
	set_last_exit_status(result);
	ft_free_and_close(data, cmd);
	exit(result);
}

void	ft_exec_start(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	struct stat	path_stat;
	char		*exec_path;

	if (!cmd->argv[0])
	{
		ft_free_and_close(data, cmd);
		exit(0);
	}
	if ((is_builtin(cmd)))
		ft_exec_builtin_ctrl(cmd, data, env_list);
	ft_exec_ctrl(cmd, data, &exec_path, &path_stat);
	if (!exec_path)
	{
		exec_path = find_executable(cmd->argv[0], *env_list);
		if (!exec_path)
			ft_not_executable(cmd, data, NULL);
	}
	execve(exec_path, cmd->argv, data->env);
	perror("execve");
	ft_free_and_close(data, cmd);
	exit(127);
}
