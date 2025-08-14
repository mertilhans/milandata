#include "shell.h"

static t_parser	*tokenize_and_parse(char *line, t_env *env_list)
{
	t_token		*tokens;
	t_parser	*cmd_list;

	tokens = tokenize_input(line);
	if (!tokens)
	{
		setup_interactive_signals();
		return (NULL);
	}
	cmd_list = parse_tokens(tokens, env_list);
	if (!cmd_list)
	{
		setup_interactive_signals();
		return (NULL);
	}
	return (cmd_list);
}

static void	execute_pipeline(t_parser *cmd_list, t_env **env_list, char **env)
{
	char	**updated_env;

	updated_env = env_list_to_array(*env_list);
	if (updated_env)
	{
		execute_cmds(cmd_list, updated_env, env_list);
		cleanup_env_array(updated_env);
	}
	else
		execute_cmds(cmd_list, env, env_list);
}

int	process_command_line(char *line, t_env **env_list, char **env)
{
	t_parser	*cmd_list;

	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	if (!line || *line == '\0')
		return (0);
	cmd_list = tokenize_and_parse(line, *env_list);
	if (cmd_list)
		execute_pipeline(cmd_list, env_list, env);
	setup_interactive_signals();
	return (0);
}