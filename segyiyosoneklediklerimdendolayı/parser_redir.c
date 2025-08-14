#include "shell.h"
#include <stdlib.h> // exit, EXIT_FAILURE
#include <stdio.h> // perror
char	*remove_quotes(char *delimiter);
int		has_quote_chars(char *delimiter);

void	ft_temp_next(t_redirection *new_redir, t_parser *cmd)
{
	t_redirection	*temp;

	temp = cmd->redirs;
	while (temp->next)
		temp = temp->next;
	temp->next = new_redir;
}

void	init_redirection(t_redirection *r, t_redir_type t, char *f, int n)
{
	r->filename = ft_strdup(f);
	if (!r->filename)
	{
		perror("strdup failed for redirection filename");
		gc_free(r);
		exit(EXIT_FAILURE);
	}
	r->type = t;
	r->no_expand = n;
	r->next = NULL;
}

void	add_redirection(t_parser *cmd, t_redir_type type, char *filename)
{
	t_redirection	*new_redir;

	new_redir = gb_malloc(sizeof(t_redirection));
	if (!new_redir)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	init_redirection(new_redir, type, filename, 0);
	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
		ft_temp_next(new_redir, cmd);
}

void	ft_redir_heredoc(t_token *tokens, t_parser *cmd)
{
	char	*delimiter;
	char	*clean_delimiter;
	int		quotes_found;

	tokens = tokens->next;
	if (tokens && tokens->type == TOKEN_WORD)
	{
		delimiter = tokens->value;
		quotes_found = has_quote_chars(delimiter);
		clean_delimiter = remove_quotes(delimiter);
		if (!clean_delimiter)
		{
			cmd->parse_error = 1;
			return ;
		}
		init_redirection(cmd->redirs, REDIR_HEREDOC, clean_delimiter, quotes_found);
		gc_free(clean_delimiter);
	}
	else
	{
		cmd->parse_error = 1;
		set_last_exit_status(2);
	}
}
