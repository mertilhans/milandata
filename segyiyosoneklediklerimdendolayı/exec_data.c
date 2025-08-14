#include "shell.h"
#include <unistd.h> 
#include <stdio.h> 

int	count_commands(t_parser *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static void	cleanup_fds_on_error(int stdin_bak, int stdout_bak)
{
	if (stdin_bak != -1)
		close(stdin_bak);
	if (stdout_bak != -1)
		close(stdout_bak);
}

int	data_init(t_exec_data *data, t_parser *cmd_list, char **env)
{
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	if (data->original_stdin == -1 || data->original_stdout == -1)
	{
		perror("dup failed");
		cleanup_fds_on_error(data->original_stdin, data->original_stdout);
		return (1);
	}
	data->pids_size = count_commands(cmd_list);
	data->pids = NULL;
	data->i = 0;
	cmd_list->fd_i = data->original_stdin;
	cmd_list->fd_o = data->original_stdout;
	data->env = env;
	data->in_fd = STDIN_FILENO;
	data->pipefd[0] = -1;
	data->pipefd[1] = -1;
	return (0);
}

static void	cleanup_remaining_heredocs(t_parser *cmd_list)
{
	t_parser	*current_cmd;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		if (current_cmd->heredoc_fd != -1 && current_cmd->heredoc_fd != -2)
		{
			close(current_cmd->heredoc_fd);
			current_cmd->heredoc_fd = -1;
		}
		current_cmd = current_cmd->next;
	}
}

int	ft_data_pids(t_parser *cmd_list, t_exec_data *data, t_env **env_list)
{
	data->pids = gb_malloc(sizeof(int) * data->pids_size);
	if (!data->pids)
	{
		perror("pids error");
		cleanup_remaining_heredocs(cmd_list);
		dup2(data->original_stdin, STDIN_FILENO);
		close(data->original_stdin);
		dup2(data->original_stdout, STDOUT_FILENO);
		close(data->original_stdout);
		return (1);
	}
	execute_loop(cmd_list, data, env_list);
	return (0);
}
