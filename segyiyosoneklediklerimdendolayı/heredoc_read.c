#include "shell.h"
#include <unistd.h> 
static int	handle_read_error(ssize_t bytes, int at_start, size_t len)
{
	if (bytes == 0)
	{
		if (at_start && len == 0)
			return (-1);
		return (1);
	}
	if (errno == EINTR)
	{
		errno = EINTR;
		return (-1);
	}
	return (-1);
}

static char	*resize_line_buffer(char *line, size_t *line_size, size_t line_len)
{
	char	*new_line;

	*line_size *= 2;
	new_line = gb_malloc(*line_size);
	if (!new_line)
	{
		gc_free(line);
		return (NULL);
	}
	memcpy(new_line, line, line_len);
	gc_free(line);
	return (new_line);
}

char	*heredoc_readline(const char *prompt)
{
	char	*line;
	size_t	line_size;
	size_t	line_len;
	char	ch;

	if (prompt)
		write(STDOUT_FILENO, prompt, strlen(prompt));
	line_size = 128;
	line = gb_malloc(line_size);
	if (!line)
		return (NULL);
	line_len = 0;
	while (1)
	{
		if (read(STDIN_FILENO, &ch, 1) <= 0)
			if (handle_read_error(read(STDIN_FILENO, &ch, 1), 1, line_len) == -1)
				return (gc_free(line), NULL);
		if (ch == '\n')
			return (line[line_len] = '\0', line);
		if (line_len + 1 >= line_size)
			line = resize_line_buffer(line, &line_size, line_len);
		if (!line)
			return (NULL);
		line[line_len++] = ch;
	}
}

char	*read_single_heredoc_block(char *delimiter)
{
	t_heredoc_buffer	buf;
	char				*result;

	buf.content = ft_strdup("");
	if (!buf.content)
		return (NULL);
	buf.delimiter_len = ft_strlen(delimiter);
	result = readline_loop(&buf, delimiter);
	if (!result && get_last_exit_status() == 130)
	{
		gc_free(buf.content);
		return (NULL);
	}
	return (result);
}

char	*read_heredoc_with_expand(char *delimiter, t_env *env_list)
{
	t_heredoc_buffer	buf;
	char				*result;

	buf.content = ft_strdup("");
	if (!buf.content)
		return (NULL);
	buf.delimiter_len = ft_strlen(delimiter);
	result = readline_loop_expand(&buf, delimiter, env_list);
	if (!result && get_last_exit_status() == 130)
	{
		gc_free(buf.content);
		return (NULL);
	}
	return (result);
}