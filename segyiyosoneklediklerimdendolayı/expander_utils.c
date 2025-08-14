#include "shell.h"

static void	quotes_loop(char *str, t_builder *b, t_env *env_list)
{
	int		i;
	char	quote_char;
	char	*expanded;

	i = 0;
	quote_char = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote_char == 0)
			quote_char = str[i++];
		else if (quote_char != 0 && quote_char == str[i])
		{
			quote_char = 0;
			i++;
		}
		else if (str[i] == '$' && quote_char != '\'')
		{
			expanded = handle_dollar(str, &i, env_list);
			builder_append_str(b, expanded);
			gc_free(expanded);
		}
		else
			builder_push_char(b, str[i++]);
	}
}

char	*expand_with_quotes(char *str, t_env *env_list)
{
	t_builder	builder;

	if (!str)
		return (NULL);
	builder_init(&builder, ft_strlen(str) + 1);
	if (!builder.str)
		return (NULL);
	quotes_loop(str, &builder, env_list);
	return (builder.str);
}

static void	heredoc_loop(char *str, t_builder *b, t_env *env_list)
{
	int		i;
	char	*expanded;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			expanded = handle_dollar(str, &i, env_list);
			builder_append_str(b, expanded);
			gc_free(expanded);
		}
		else
			builder_push_char(b, str[i++]);
	}
}

char	*expand_heredoc_line(char *str, t_env *env_list)
{
	t_builder	builder;

	if (!str)
		return (NULL);
	builder_init(&builder, ft_strlen(str) + 1);
	if (!builder.str)
		return (NULL);
	heredoc_loop(str, &builder, env_list);
	return (builder.str);
}