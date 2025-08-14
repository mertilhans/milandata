#include "shell.h"

static int	pre_parse_checks(t_token **tokens)
{
	if (ft_syn(*tokens))
		return (1);
	if (token_ctrl(*tokens) && !(*tokens)->next)
	{
		printf("bash: syntax error near unexpected token `newline'\n");
		set_last_exit_status(2);
		return (1);
	}
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		set_last_exit_status(2);
		*tokens = (*tokens)->next;
	}
	return (0);
}

static void	main_parse_loop(t_token *tokens, t_all *all)
{
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (pre_parse_checks(&tokens))
			return ;
		if (!tokens || tokens->type == TOKEN_EOF)
			break ;
		tokens = ft_control_token(tokens, all);
		if (all->cmd_list && all->cmd_list->parse_error)
		{
			set_last_exit_status(2);
			return ;
		}
	}
}

t_parser	*parse_tokens(t_token *tokens, t_env *env_list)
{
	t_all	*all;

	all = gb_malloc(sizeof(t_all));
	if (!all)
		exit(EXIT_FAILURE);
	all->cmd_list = NULL;
	all->last_cmd = NULL;
	all->env_list = env_list;
	main_parse_loop(tokens, all);
	return (all->cmd_list);
}