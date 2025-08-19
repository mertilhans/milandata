/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 04:14:54 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/19 19:42:33 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <readline/readline.h>
#include <signal.h>

t_env	*initialize_shell(char **env)
{
	t_env		*env_list;
	t_export	**export_list;

	env_list = init_env(env);
	export_list = get_export_list();
	*export_list = init_export_from_env(env_list);
	init_shell_variables(&env_list);
	setup_interactive_signals();
	return (env_list);
}

int	main(int ac, char **av, char **env)
{
	t_env	*env_list;

	signal(SIGPIPE, SIG_IGN);
	if (ac != 1)
	{
		ft_putendl_fd("No usage syntax\n", 2);
		return (1);
	}
	(void)av;
	set_last_exit_status(0);
	env_list = initialize_shell(env);
	shell_loop(env_list, env);
	rl_clear_history();
	env_gb_free_all();
	gb_free_all();
	return (get_last_exit_status());
}
