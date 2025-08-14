#include "shell.h"

static int	handle_heredoc_interrupt(void)
{
	write(STDOUT_FILENO, "", 0);
	set_last_exit_status(130);
	return (0);
}

static int	handle_heredoc_eof(const char *delimiter, t_heredoc_buffer *buf)
{
	(void)buf;
	printf("bash: warning: here-document ... (wanted `%s')\n", delimiter);
	set_last_exit_status(0);
	return (1);
}

char	*readline_loop(t_heredoc_buffer *buf, const char *delimiter)
{
	heredoc_signals();
	while (1)
	{
		errno = 0;
		buf->line = heredoc_readline("> ");
		if (!buf->line)
		{
			if (errno == EINTR)
				return (handle_heredoc_interrupt(), NULL);
			else
				return (handle_heredoc_eof(delimiter, buf), buf->content);
		}
		if ((size_t)ft_strlen(buf->line) == buf->delimiter_len
			&& ft_strcmp(buf->line, delimiter) == 0)
		{
			gc_free(buf->line);
			break ;
		}
		if (!heredoc_append_line(buf))
			return (gc_free(buf->line), gc_free(buf->content), NULL);
		gc_free(buf->line);
	}
	return (buf->content);
}

char	*readline_loop_expand(t_heredoc_buffer *buf, const char *del, t_env *env)
{
	heredoc_signals();
	while (1)
	{
		errno = 0;
		buf->line = heredoc_readline("> ");
		if (!buf->line)
		{
			if (errno == EINTR)
				return (handle_heredoc_interrupt(), NULL);
			else
				return (handle_heredoc_eof(del, buf), buf->content);
		}
		if ((size_t)ft_strlen(buf->line) == buf->delimiter_len
			&& ft_strcmp(buf->line, del) == 0)
		{
			gc_free(buf->line);
			break ;
		}
		if (!heredoc_append_expanded(buf, env))
			return (gc_free(buf->line), gc_free(buf->content), NULL);
		gc_free(buf->line);
	}
	return (buf->content);
}