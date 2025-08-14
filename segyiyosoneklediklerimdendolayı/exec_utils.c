#include "shell.h"
#include <unistd.h> 

void	ft_putendl_fd(char *str, int fd)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}

void	close_all_fds_except_std(t_parser *cmd)
{
	t_parser	*cmd_list;

	cmd_list = cmd;
	while (cmd_list)
	{
		if (cmd_list->heredoc_fd != -2 && cmd_list->heredoc_fd != -1)
		{
			close(cmd_list->heredoc_fd);
			cmd_list->heredoc_fd = -1;
		}
		cmd_list = cmd_list->next;
	}
}

void	close_fd(t_exec_data *data)
{
	if (data->original_stdin != -1)
		close(data->original_stdin);
	if (data->original_stdout != -1)
		close(data->original_stdout);
}

void	wait_pids(t_exec_data *data)
{
	int	status;
	int	j;

	j = 0;
	while (j < data->i)
	{
		waitpid(data->pids[j], &status, 0);
		if (j == data->i - 1)
			set_last_exit_status(calculate_exit_status(status));
		j++;
	}
}

void	finish_fd(t_parser *cmd_list, t_exec_data *data)
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
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}