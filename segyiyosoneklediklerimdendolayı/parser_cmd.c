#include "shell.h"

void	ft_loop(t_token **tokens, t_parser *cmd, int *argc, t_all *all);

void	ft_clean_init(t_parser *cmd)
{
	cmd->argv_cap = 1;
	cmd->argv = gb_malloc(sizeof(char *) * cmd->argv_cap);
	if (!cmd->argv)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->parse_error = 0;
	cmd->heredoc_fd = -2;
}

void	init_parser_cmd(t_parser *cmd)
{
	if (!cmd)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	ft_clean_init(cmd);
}

void	finalize_cmd(t_parser *cmd, int argc, t_all *data)
{
	cmd->argv[argc] = NULL;
	if (!(data->cmd_list))
		data->cmd_list = cmd;
	else
		(data->last_cmd)->next = cmd;
	data->last_cmd = cmd;
}

t_token	*ft_control_token(t_token *tokens, t_all *data)
{
	int			argc;
	t_parser	*cmd;

	argc = 0;
	cmd = gb_malloc(sizeof(t_parser));
	init_parser_cmd(cmd);
	while (tokens && tokens->type != TOKEN_PIPE && tokens->type != TOKEN_EOF)
		ft_loop(&tokens, cmd, &argc, data);
	finalize_cmd(cmd, argc, data);
	if (tokens && tokens->type == TOKEN_PIPE)
		tokens = tokens->next;
	return (tokens);
}