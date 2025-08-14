#include "shell.h"
t_env *g_env_list = NULL;

static void	handle_line(char *line, t_env **env_list, char **env)
{
	if (line && line[0] != '\0')
	{
		add_history(line);
		process_command_line(line, env_list, env);
	}
	free(line);
}

void	shell_loop(t_env *env_list, char **env)
{
	char	*line;

	while (1)
	{
		setup_interactive_signals();
		line = readline("MiniShell->>>    ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		handle_line(line, &env_list, env);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*env_list;

	(void)ac;
	(void)av;
	set_last_exit_status(0);
	env_list = init_env(env);
	g_env_list = env_list;
	init_shell_variables(&env_list);
	shell_loop(env_list, env);
	rl_clear_history();
	env_gb_free_all();
	gb_free_all();
	return (get_last_exit_status());
}