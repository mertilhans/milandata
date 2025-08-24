/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 04:15:36 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/24 15:32:35 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

static int	handle_line_input(char *line, t_env **env_list, char **env)
{
	char *res;
	res = ft_strdup(line);
	free(line);
	if (!res)
	{
		ft_putendl_fd("exit\n", 1);
		return (1);
	}
	if (res[0] == '\0')
	{
		gc_free(res);
		return (2);
	}
	if (ft_strlen(res) > 0)
		process_command_line(res, env_list, env);
	gc_free(res);
	return (0);
}

void	shell_loop(t_env *env_list, char **env)
{
	char	*line;
	int		status;

	while (1)
	{
		if (get_last_exit_status() == 130)
			write(1, "\n", 1);
		if (get_last_exit_status() == 131)
			write(1, "Quit (core dumped)\n", 19);
		rl_on_new_line();
		rl_replace_line("", 0);
		line = readline("MiniShell->>>   ");
		status = handle_line_input(line, &env_list, env);
		if (status == 1)
			break ;
		else if (status == 2)
			continue ;
		setup_interactive_signals();
	}
}

int	process_command_line(char *line, t_env **env_list, char **env)
{
	t_parser	*cmd_list;
	char		**updated_env;

	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	if (!line || *line == '\0')
		return (0);
	add_history(line);
	cmd_list = parse_line(line, *env_list);
	if (!cmd_list)
		return (0);
	updated_env = env_list_to_array(*env_list);
	if (updated_env)
	{
		execute_cmds(cmd_list, updated_env, env_list);
		cleanup_env_array(updated_env);
	}
	else
		execute_cmds(cmd_list, env, env_list);
	setup_interactive_signals();
	return (0);
}
