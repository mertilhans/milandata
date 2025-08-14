#include "shell.h"
#include <string.h> 
#include <unistd.h> 

int	is_builtin(t_parser *cmd)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	name = cmd->argv[0];
	if (strcmp(name, "exit") == 0 || strcmp(name, "cd") == 0)
		return (1);
	if (strcmp(name, "pwd") == 0 || strcmp(name, "echo") == 0)
		return (1);
	if (strcmp(name, "export") == 0 || strcmp(name, "unset") == 0)
		return (1);
	if (strcmp(name, "env") == 0)
		return (1);
	return (0);
}

static int	ft_builtin_call_2(t_parser *cmd, t_env **env_list)
{
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd, env_list), get_last_exit_status());
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(), get_last_exit_status());
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd, env_list), get_last_exit_status());
	if (ft_strcmp(cmd->argv[0], "env") == 0)
	{
		if (cmd->argv[1])
		{
			ft_putendl_fd(" : No such file or directory\n", 2);
			return (127);
		}
		builtin_env(*env_list);
		return (0);
	}
	return (-1);
}

int	ft_builtin_call(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	int	result;

	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (built_echo(cmd), get_last_exit_status());
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (built_cd(cmd));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
	{
		cmd->fd_i = data->original_stdin;
		cmd->fd_o = data->original_stdout;
		builtin_exit(cmd);
		return (0);
	}
	result = ft_builtin_call_2(cmd, env_list);
	if (result != -1)
		return (result);
	return (-1);
}

static void	restore_fds(int stdin_bak, int stdout_bak)
{
	if (stdin_bak != -1)
	{
		dup2(stdin_bak, STDIN_FILENO);
		close(stdin_bak);
	}
	if (stdout_bak != -1)
	{
		dup2(stdout_bak, STDOUT_FILENO);
		close(stdout_bak);
	}
}

int	n_next_or_built(t_parser *cmd_list, t_exec_data *data, t_env **env_list)
{
	setup_interactive_signals();
	if (cmd_list->heredoc_fd != -1 && cmd_list->heredoc_fd != -2)
	{
		dup2(cmd_list->heredoc_fd, STDIN_FILENO);
		close(cmd_list->heredoc_fd);
		cmd_list->heredoc_fd = -1;
	}
	if (setup_file_redirections(cmd_list) != 0)
		set_last_exit_status(1);
	else
		ft_builtin_call(cmd_list, data, env_list);
	restore_fds(data->original_stdin, data->original_stdout);
	setup_interactive_signals();
	return (get_last_exit_status());
}