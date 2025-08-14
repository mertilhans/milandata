#include "shell.h"

int	has_quote_chars(char *delimiter)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(delimiter);
	while (i < len)
	{
		if (delimiter[i] == '"' || delimiter[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char	*remove_quotes(char *delimiter)
{
	char	*clean_delimiter;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(delimiter);
	clean_delimiter = gb_malloc(len + 1);
	if (!clean_delimiter)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (delimiter[i] != '"' && delimiter[i] != '\'')
			clean_delimiter[j++] = delimiter[i];
		i++;
	}
	clean_delimiter[j] = '\0';
	return (clean_delimiter);
}

int	token_ctrl(t_token *tokens)
{
	if (tokens->type == TOKEN_REDIR_IN)
		return (1);
	if (tokens->type == TOKEN_REDIR_OUT)
		return (1);
	if (tokens->type == TOKEN_REDIR_APPEND)
		return (1);
	if (tokens->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	ft_syn(t_token *tokens)
{
	t_token	*token;

	token = tokens;
	while (token)
	{
		if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_REDIR_APPEND
			|| token->type == TOKEN_REDIR_IN || token->type == TOKEN_HEREDOC)
		{
			if (!token->next)
			{
				printf("bash: syntax error near unexpected token `newline'\n");
				return (1);
			}
		}
		token = token->next;
	}
	return (0);
}
int	should_split_expansion(char *original_token, char *expanded)
{
	int	has_dollar_expansion;
	int	is_quoted;

	has_dollar_expansion = ft_strchr(original_token, '$') != NULL; // if atçam buraya şimdilik deniyom
	is_quoted = ft_strchr(original_token, '"')
		|| ft_strchr(original_token, '\'');
	if (!has_dollar_expansion || !expanded || is_quoted)
		return (0);
	if (!ft_strchr(expanded, ' ') && !ft_strchr(expanded, '\t'))
		return (0);
	return (1);
}