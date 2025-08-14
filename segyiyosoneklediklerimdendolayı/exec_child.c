#include "shell.h"
#include <unistd.h> 

static void	child_redir_and_exec(t_parser *cmd, t_exec_data *d, t_env **e_list)
{
	if (ft_redir_ctrl(cmd) == -1)
	{
		close_fd(d);
		close_all_fds_except_std(cmd);
		env_gb_free_all();
		gb_free_all();
		exit(1);
	}
	ft_exec_start(cmd, d, e_list);
}

void	child_process_exec(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	setup_child_signals();
	if (data->in_fd != STDIN_FILENO)
	{
		if (dup2(data->in_fd, STDIN_FILENO) == -1)
			exit(1);
		close(data->in_fd);
	}
	if (cmd->next && data->pipefd[1] != -1)
	{
		if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(data->pipefd[0]);
		close(data->pipefd[1]);
	}
	child_redir_and_exec(cmd, data, env_list);
}

static void	handle_exec_error(char *name, char *msg, int code, t_exec_data *d)
{
	ft_putendl_fd("bash: ", 2);
	ft_putendl_fd(name, 2);
	ft_putendl_fd(msg, 2);
	close_fd(d);
	gb_free_all();
	env_gb_free_all();
	exit(code);
}

static void	check_path_and_execute(t_parser *cmd, t_exec_data *d, t_env **e)
{
	char		*exec_path;
	struct stat	path_stat;
        (void)e; // buraya döncez tam olmamış olabilir
	if (ft_strchr(cmd->argv[0], '/'))
	{
		if (stat(cmd->argv[0], &path_stat) != 0)
			handle_exec_error(cmd->argv[0], ": No such file or directory\n", 127, d);
		if (S_ISDIR(path_stat.st_mode))
			handle_exec_error(cmd->argv[0], ": is a directory\n", 126, d);
		if (access(cmd->argv[0], X_OK) != 0)
			handle_exec_error(cmd->argv[0], ": Permission denied\n", 126, d);
		exec_path = cmd->argv[0];
	}
	else
	{
		exec_path = find_executable(cmd->argv[0]);
		if (!exec_path)
			handle_exec_error(cmd->argv[0], ": command not found\n", 127, d);
	}
	execve(exec_path, cmd->argv, d->env);
	handle_exec_error(exec_path, ": execve failed\n", 127, d);
}

void	ft_exec_start(t_parser *cmd, t_exec_data *data, t_env **env_list)
{
	if (!cmd->argv[0] || cmd->argv[0][0] == '\0')
		exit(0);
	if (is_builtin(cmd))
	{
		exit(ft_builtin_call(cmd, data, env_list));
	}
	check_path_and_execute(cmd, data, env_list);
}