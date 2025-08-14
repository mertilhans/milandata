#include "shell.h"
#include <unistd.h> 
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

static int	fork_pipe_exec(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		child_process_exec(cmd, data, env_list);
		exit(1);
	}
	return (pid);
}

static int	setup_and_fork(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	int	pid;

	if (setup_pipe_if_needed(cmd, data) == -1)
		return (-1);
	pid = fork_pipe_exec(cmd, data, env_list);
	if (pid == -1)
	{
		if (cmd->next)
		{
			close(data->pipefd[0]);
			close(data->pipefd[1]);
		}
		return (-1);
	}
	data->pids[data->i++] = pid;
	return (pid);
}

static int	process_command(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	data->pipefd[0] = -1;
	data->pipefd[1] = -1;
	if (setup_and_fork(cmd, data, env_list) == -1)
		return (0);
	if (cmd->heredoc_fd != -1 && cmd->heredoc_fd != -2)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (data->in_fd != STDIN_FILENO)
		close(data->in_fd);
	if (cmd->next)
	{
		close(data->pipefd[1]);
		data->in_fd = data->pipefd[0];
	}
	else
		data->in_fd = STDIN_FILENO;
	return (1);
}

void	execute_loop(t_parser *cmd_list, t_exec_data *data, t_env **env_list)
{
	t_parser	*cmd;

	data->in_fd = STDIN_FILENO;
	cmd = cmd_list;
	while (cmd)
	{
		if (!process_command(cmd, data, env_list))
			break ;
		cmd = cmd->next;
	}
	if (data->in_fd != STDIN_FILENO)
		close(data->in_fd);
	wait_pids(data);
	gc_free(data->pids);
}