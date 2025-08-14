#include "shell.h"
#include "fcntl.h"

int	redir_in(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redir_out(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	redir_append(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	apply_heredoc_redir(t_parser *cmd)
{
	if (cmd->heredoc_fd != -1 && cmd->heredoc_fd != -2)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			close(cmd->heredoc_fd);
			return (-1);
		}
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	return (0);
}

int	ft_redir_ctrl(t_parser *cmd)
{
	t_redirection	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN && redir_in(redir) != 0)
			return (-1);
		else if (redir->type == REDIR_OUT && redir_out(redir) != 0)
			return (-1);
		else if (redir->type == REDIR_APPEND && redir_append(redir) != 0)
			return (-1);
		redir = redir->next;
	}
	if (apply_heredoc_redir(cmd) != 0)
		return (-1);
	return (0);
}