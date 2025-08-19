/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 05:49:15 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/19 21:29:28 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <unistd.h>

void	ft_putendl_fd(char *str, int fd)
{
	if (!str)
		return ;
	while (*str)
		write(fd, str++, 1);
}

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

int	get_argv_count(char **argv)
{
	int	count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}

char	*copy_dir(const char *dir, int len)
{
	char	*res;
	int		i;

	i = 0;
	res = gb_malloc(len + 1);
	if (!res)
		return (NULL);
	while (i < len)
	{
		res[i] = dir[i];
		i++;
	}
	res[len] = '\0';
	return (res);
}
void	close_pipe_fds(t_exec_data *data)
{
	if (data->pipefd[0] > 2)
	{
		close(data->pipefd[0]);
		data->pipefd[0] = -1;
	}
	if (data->pipefd[1] > 2)
	{
		close(data->pipefd[1]);
		data->pipefd[1] = -1;
	}
}


void	ft_free_and_close(t_exec_data *data, t_parser *cmd)
{
	if (data->original_stdin > 2)
	{
		close(data->original_stdin);
		data->original_stdin = -1;
	}
	if (data->original_stdout > 2)
	{
		close(data->original_stdout);
		data->original_stdout = -1;
	}
	close_pipe_fds(data);
	if (data->in_fd != STDIN_FILENO && data->in_fd > 2)
	{
		close(data->in_fd);
		data->in_fd = STDIN_FILENO;
	}
	close_heredoc_fd(cmd);
	gb_free_all();
	env_gb_free_all();
}
void	close_all_heredoc_fds(t_parser *cmd_list)
{
	t_parser	*current;
	
	current = cmd_list;
	while (current)
	{
		if (current->heredoc_fd > 2)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = current->next;
	}
}
