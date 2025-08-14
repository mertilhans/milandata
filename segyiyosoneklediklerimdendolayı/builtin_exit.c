#include "shell.h"
static void	exit_numeric_error(t_parser *cmd)
{
	printf("bash: exit: %s: numeric argument required\n", cmd->argv[1]);
	printf("exit\n");
	close(cmd->fd_i);
	close(cmd->fd_o);
	close_all_fds_except_std(cmd);
	gb_free_all();
	env_gb_free_all();
	exit(2);
}

void	builtin_exit(t_parser *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->argv[1] && cmd->argv[2])
	{
		printf("bash: exit: too many arguments\n");
		set_last_exit_status(1);
		return ;
	}
	if (cmd->argv[1])
	{
		if (!is_numeric_string(cmd->argv[1]))
			exit_numeric_error(cmd);
		exit_code = ft_atoi(cmd->argv[1]);
		exit_code = ((exit_code % 256) + 256) % 256;
	}
	printf("exit\n");
	close(cmd->fd_i);
	close(cmd->fd_o);
	close_all_fds_except_std(cmd);
	gb_free_all();
	env_gb_free_all();
	exit(exit_code);
}