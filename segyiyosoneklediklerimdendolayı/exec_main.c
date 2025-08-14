#include "shell.h"
#include <unistd.h> 
static int	handle_heredoc_failure(t_exec_data *data)
{
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
	setup_interactive_signals();
	return (get_last_exit_status());
}

static int	execute_single_builtin(t_parser *c, t_exec_data *d, t_env **e)
{
	if (d->pids_size == 1 && is_builtin(c))
	{
		set_last_exit_status(n_next_or_built(c, d, e));
		return (1);
	}
	return (0);
}

int	execute_cmds(t_parser *cmd_list, char **env, t_env **env_list)
{
	t_exec_data	data;

	if (data_init(&data, cmd_list, env))
		return (1);
	if (heredoc_handle(cmd_list, &data, *env_list))
		return (handle_heredoc_failure(&data));
	setup_interactive_signals();
	if (execute_single_builtin(cmd_list, &data, env_list))
		return (get_last_exit_status());
	setup_parent_execution_signals();
	if (ft_data_pids(cmd_list, &data, env_list))
	{
		setup_interactive_signals();
		return (1);
	}
	setup_interactive_signals();
	finish_fd(cmd_list, &data);
	return (get_last_exit_status());
}