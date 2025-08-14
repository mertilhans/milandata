#include "shell.h"

void	ft_loop_3(t_token **tokens, t_parser *cmd, int *argc, t_all *all)
{
	char	*original_token;
	char	*expanded;

	if ((*tokens)->type != TOKEN_WORD)
		return ;
	if (*argc + 1 >= cmd->argv_cap)
		expand_argv_capacity(cmd, argc);
	original_token = (*tokens)->value;
	expanded = expand_with_quotes(original_token, all->env_list);
	if (*argc == 0 && is_empty_expansion(original_token, expanded))
	{
		if (expanded)
			gc_free(expanded);
		return ;
	}
	if (should_split_expansion(original_token, expanded))
		handle_split_expansion(cmd, argc, original_token, expanded);
	else
		handle_normal_expansion(cmd, argc, *tokens, expanded);
}

void	ft_loop_2(t_token **tokens, t_parser *cmd, int *argc, t_all *all)
{
	if ((*tokens)->type == TOKEN_REDIR_APPEND)
	{
		add_redirection(cmd, REDIR_APPEND, (*tokens)->next->value);
		if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
			*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == TOKEN_HEREDOC)
	{
		ft_redir_heredoc(*tokens, cmd);
		if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
			*tokens = (*tokens)->next;
	}
	else
		ft_loop_3(tokens, cmd, argc, all);
}

void	ft_loop(t_token **tokens, t_parser *cmd, int *argc, t_all *all)
{
	if (cmd->parse_error)
	{
		printf("bash: syntax error near unexpected token `newline'\n");
		set_last_exit_status(2);
		return ;
	}
	if ((*tokens)->type == TOKEN_REDIR_IN)
	{
		add_redirection(cmd, REDIR_IN, (*tokens)->next->value);
		if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
			*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == TOKEN_REDIR_OUT)
	{
		add_redirection(cmd, REDIR_OUT, (*tokens)->next->value);
		if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
			*tokens = (*tokens)->next;
	}
	else
		ft_loop_2(tokens, cmd, argc, all);
	*tokens = (*tokens)->next;
}