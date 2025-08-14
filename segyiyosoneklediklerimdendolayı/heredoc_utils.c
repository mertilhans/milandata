#include "shell.h"

int heredoc_append_line(t_heredoc_buffer *buf)
{
	size_t line_len;
	if (!buf || !buf->line || !buf->content)
		return (0);
	line_len = ft_strlen(buf->line);
	buf->line_with_nl = gb_malloc(line_len + 2);
	if (!buf->line_with_nl)
		return (0);
	ft_strcpy(buf->line_with_nl, buf->line);
	buf->line_with_nl[line_len] = '\n';
	buf->line_with_nl[line_len + 1] = '\0';
	buf->new_content = gb_malloc(ft_strlen(buf->content)
			+ ft_strlen(buf->line_with_nl) + 1);
	if (!buf->new_content)
	{
		gc_free(buf->line_with_nl);
		return (0);
	}
	ft_strcpy(buf->new_content, buf->content);
	ft_strcat(buf->new_content, buf->line_with_nl);
	gc_free(buf->content);
	gc_free(buf->line_with_nl);
	buf->content = buf->new_content;
	return (1);
}

int heredoc_append_expanded(t_heredoc_buffer *buf, t_env *env_list)
{
	char    *expanded_line;
	char    *old_line;
	int     result;

	if (!buf || !buf->line || !buf->content)
		return (0);
	old_line = buf->line;
	expanded_line = expand_heredoc_line(buf->line, env_list);
	if (!expanded_line)
		expanded_line = ft_strdup(buf->line);
	buf->line = expanded_line;
	result = heredoc_append_line(buf);
	buf->line = old_line;
	if (expanded_line != old_line)
		gc_free(expanded_line);
	return (result);
}

static int	write_heredoc_to_pipe(int *pipefd, t_heredoc_data *data)
{
	if (pipe(pipefd) == -1)
	{
		perror("heredoc pipe");
		gc_free(data->heredoc_content);
		return (-1);
	}
	if (data->heredoc_len > 0)
		write(pipefd[1], data->heredoc_content, data->heredoc_len);
	close(pipefd[1]);
	gc_free(data->heredoc_content);
	return (pipefd[0]);
}

int	ft_h_built_expand(t_redirection *redir, t_heredoc_data *data, t_env *e_list)
{
	int	pipefd[2];

	if (redir->no_expand)
		data->heredoc_content = read_single_heredoc_block(redir->filename);
	else
		data->heredoc_content = read_heredoc_with_expand(redir->filename, e_list);
	if (!data->heredoc_content)
	{
		if (get_last_exit_status() == 130)
			return (-1);
		data->heredoc_content = ft_strdup("");
	}
	data->heredoc_len = ft_strlen(data->heredoc_content);
	data->last_heredoc_fd = write_heredoc_to_pipe(pipefd, data);
	return (data->last_heredoc_fd);
}