/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 05:50:22 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/20 22:39:17 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

char	*readline_loop(t_heredoc_buffer *buf, const char *delimiter)
{
	heredoc_signals();
	while (1)
	{
		buf->line = readline("> ");
		if (g_heredoc_interrupted)
		{
			if (!buf->line) // readline input buffer boşsa, bash gibi newline bas
				write(STDOUT_FILENO, "\n", 1);
			if (buf->line)
				free(buf->line);
			buf->line = NULL;
			setup_interactive_signals();
			g_heredoc_interrupted = 0;
			set_last_exit_status(130);
			return (NULL);
		}
		if (!buf->line)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			set_last_exit_status(0);
			setup_interactive_signals();
			return (buf->content);
		}
		if (check_delimiter(buf, delimiter))
		{
			setup_interactive_signals();
			break ;
		}
		if (!process_line(buf, NULL, 0))
		{
			setup_interactive_signals();
			return (NULL);
		}
	}
	setup_interactive_signals();
	return (buf->content);
}

char	*readline_loop_expand(t_heredoc_buffer *buf, const char *delimiter,
		t_env *env_list)
{
	heredoc_signals();
	while (1)
	{
		buf->line = readline("> ");
		if (g_heredoc_interrupted)
		{
			if (!buf->line) // readline input buffer boşsa, bash gibi newline bas
				write(STDOUT_FILENO, "\n", 1);
			if (buf->line)
				free(buf->line);
			buf->line = NULL;
			setup_interactive_signals();
			g_heredoc_interrupted = 0;
			set_last_exit_status(130);
			return (NULL);
		}
		if (!buf->line)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			set_last_exit_status(0);
			setup_interactive_signals();
			return (buf->content);
		}
		if (check_delimiter(buf, delimiter))
		{
			setup_interactive_signals();
			break ;
		}
		if (!process_line(buf, env_list, 1))
		{
			setup_interactive_signals();
			return (NULL);
		}
	}
	setup_interactive_signals();
	return (buf->content);
}

int	process_null_line(t_heredoc_buffer *buf, const char *delimiter,
		char **result)
{
	if (get_last_exit_status() == 130)
	{
		setup_interactive_signals();
		*result = NULL;
		return (1);
	}
	printf("minishell: warning: here-document delimited by end-of-file"
		" (wanted `%s')\n", delimiter);
	set_last_exit_status(0);
	setup_interactive_signals();
	*result = buf->content;
	return (1);
}

int	process_line(t_heredoc_buffer *buf, t_env *env_list, int expand_mode)
{
	int	result;

	if (expand_mode)
		result = heredoc_append_expanded(buf, env_list);
	else
		result = heredoc_append_line(buf);
	if (!result)
	{
		not_her_app_exp(buf);
		return (0);
	}
	free(buf->line); // readline ile ayrılan belleği free ile temizle
	buf->line = NULL;
	return (1);
}

int	check_delimiter(t_heredoc_buffer *buf, const char *delimiter)
{
	if ((size_t)ft_strlen(buf->line) == buf->delimiter_len
		&& ft_strcmp(buf->line, delimiter) == 0)
	{
		free(buf->line); // readline ile ayrılan belleği free ile temizle
		buf->line = NULL;
		return (1);
	}
	return (0);
}