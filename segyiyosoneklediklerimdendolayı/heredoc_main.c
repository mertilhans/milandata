#include "shell.h"
#include <unistd.h> 

static int	h_loop(t_parser *cmd, t_heredoc_data *data, t_env *env_list)
{
	t_redirection	*current_redir;

	current_redir = cmd->redirs;
	while (current_redir)
	{
		if (current_redir->type == REDIR_HEREDOC)
		{
			if (data->last_heredoc_fd != -2 && data->last_heredoc_fd != -1)
			{
				close(data->last_heredoc_fd);
				data->last_heredoc_fd = -1;
			}
			if (ft_h_built_expand(current_redir, data, env_list) == -1)
				return (-1);
		}
		current_redir = current_redir->next;
	}
	return (0);
}

int	process_heredocs(t_parser *cmd, t_env *env_list)
{
	t_heredoc_data	h_data;

	h_data.heredoc_content = NULL;
	h_data.heredoc_len = 0;
	h_data.last_heredoc_fd = -2;
	if (h_loop(cmd, &h_data, env_list) == -1)
		return (-1);
	return (h_data.last_heredoc_fd);
}

int	heredoc_handle(t_parser *cmd_list, t_exec_data *data, t_env *env_list)
{
	t_parser	*current_cmd;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_cmd->heredoc_fd = process_heredocs(current_cmd, env_list);
		if (current_cmd->heredoc_fd == -1)
		{
			set_last_exit_status(130);
			heredoc_fd_error(cmd_list, data, current_cmd);
			setup_interactive_signals();
			rl_forced_update_display();
			return (1);
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}

int	heredoc_fd_error(t_parser *cmd_list, t_exec_data *data, t_parser *current_cmd)
{
	t_parser	*tmp_cmd;

	if (data->original_stdin != -1)
	{
		dup2(data->original_stdin, STDIN_FILENO);
		close(data->original_stdin);
	}
	if (data->original_stdout != -1)
	{
		dup2(data->original_stdout, STDOUT_FILENO);
		close(data->original_stdout);
	}
	tmp_cmd = cmd_list;
	while (tmp_cmd && tmp_cmd != current_cmd->next)
	{
		if (tmp_cmd->heredoc_fd != -1 && tmp_cmd->heredoc_fd != -2)
		{
			close(tmp_cmd->heredoc_fd);
			tmp_cmd->heredoc_fd = -1;
		}
		tmp_cmd = tmp_cmd->next;
	}
	return (1);
}