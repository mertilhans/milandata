/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 05:48:53 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/19 17:35:20 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>


void	child_process_exec(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	setup_child_signals();
	if (data->in_fd != STDIN_FILENO)
		close_fd(data);
	if (cmd->heredoc_fd == -1 || cmd->heredoc_fd == -2)
		fd_not_stdin(data);
	else
	{
		if (data->in_fd != STDIN_FILENO)
			close(data->in_fd);
	}
	if (cmd->next && data->pipefd[1] != -1)
		next_cmd_and_fd(data);
	if (ft_redir_ctrl(cmd) == -1)
	{
		ft_free_and_close(data, cmd);
		exit(1);
	}
	ft_exec_start(cmd, data, env_list);
}

void	ft_exec_builtin_ctrl(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	
	if ((data->pipefd[0] < 0))
	{
		ft_free_and_close(data, cmd);
		exit(1);
	}
	set_last_exit_status(ft_builtin_call(cmd, data, env_list));
	ft_free_and_close(data, cmd);
	exit(get_last_exit_status());
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

void	ft_exec_ctrl(t_parser *cmd, t_exec_data *data, char **exec_path,
		struct stat *path_stat)
{
	*exec_path = NULL;
	if (ft_strchr(cmd->argv[0], '/'))
	{
		if (stat(cmd->argv[0], path_stat) != 0)
			ft_not_directory(cmd, data);
		if (S_ISDIR(path_stat->st_mode))
			ft_directory(cmd, data);
		if (access(cmd->argv[0], X_OK) != 0)
			ft_permission_denied(cmd, data);
		*exec_path = cmd->argv[0];
	}
}

void	fd_not_stdin(t_exec_data *data)
{
	if (dup2(data->in_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 in_fd");
		exit(1);
	}
	if (data->in_fd != STDIN_FILENO)
		close(data->in_fd);
}
