/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 09:01:38 by husarpka          #+#    #+#             */
/*   Updated: 2025/07/20 20:38:22 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h" 


void ft_clean_init(t_parser *cmd)
{
    cmd->argv_cap = 1;
    cmd->argv = malloc(sizeof(char*) * cmd->argv_cap);
    if (!cmd->argv)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    cmd->redirs = NULL;
    cmd->next = NULL;
    cmd->parse_error = 0;
}

void add_redirection(t_parser *cmd, t_redir_type type, char *filename)
{
    t_redirection *new_redir = malloc(sizeof(t_redirection));
    if (!new_redir)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_redir->filename = filename;
    new_redir->type = type;
    new_redir->next = NULL;

    if (!cmd->redirs)
        cmd->redirs = new_redir;
    else
    {
        t_redirection *temp = cmd->redirs;
        while (temp->next)
            temp = temp->next;
        temp->next = new_redir;
    }
}

void ft_redir_in(t_token *tokens, t_parser *cmd)
{
    tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
        add_redirection(cmd, REDIR_IN, tokens->value);
    else
        cmd->parse_error = 1;
}

void ft_redir_out(t_token *tokens, t_parser *cmd)
{
    tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
        add_redirection(cmd, REDIR_OUT, tokens->value);
    else
        cmd->parse_error = 1;
}

void ft_redir_append(t_token *tokens, t_parser *cmd)
{
	tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
		add_redirection(cmd, REDIR_APPEND, tokens->value);
    else
		cmd->parse_error = 1;
}

void ft_redir_heredoc(t_token *tokens, t_parser *cmd)
{
	tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
		add_redirection(cmd, REDIR_HEREDOC, tokens->value);
    else
		cmd->parse_error = 1;
}

void ft_loop_3(t_token **tokens, t_parser *cmd, int *argc)
{
	int i;
	char **new_argv;
	
	i = 0;
	if ((*tokens)->type == TOKEN_WORD)
	{
		if (*argc + 1 >= cmd->argv_cap)
		{
			cmd->argv_cap *= 2;
			new_argv = malloc(sizeof(char*) * cmd->argv_cap);
			if (!new_argv)
			{
				perror("malloc failed");
				exit(EXIT_FAILURE);
			}
			while(i < *argc)
			{
				new_argv[i] = cmd->argv[i];
				i++;
			}
			free(cmd->argv);
			cmd->argv = new_argv;
		}
		cmd->argv[(*argc)++] = (*tokens)->value;
	}
}

void ft_loop_2(t_token **tokens, t_parser *cmd,int *argc)
{
	if ((*tokens)->type == TOKEN_REDIR_APPEND)
	{
		ft_redir_append(*tokens, cmd);
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
		ft_loop_3(tokens,cmd,argc);
	
}

void ft_loop(t_token **tokens, t_parser *cmd, int *argc)
{
    if (cmd->parse_error)
        printf("bash: syntax error near unexpected token `<'\n");

    if ((*tokens)->type == TOKEN_REDIR_IN)
    {
        ft_redir_in(*tokens, cmd);
        if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
            *tokens = (*tokens)->next;
    }
    else if ((*tokens)->type == TOKEN_REDIR_OUT)
    {
        ft_redir_out(*tokens, cmd);
        if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
            *tokens = (*tokens)->next;
    }
	else
		ft_loop_2(tokens,cmd,argc);

    *tokens = (*tokens)->next;
}

t_token *ft_control_token(t_token *tokens, t_parser **cmd_list, t_parser **last_cmd)
{
    int argc;
	t_parser *cmd;

	argc = 0;
    cmd = malloc(sizeof(t_parser));
    if (!cmd)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    ft_clean_init(cmd);
    while (tokens && tokens->type != TOKEN_PIPE && tokens->type != TOKEN_EOF)
        ft_loop(&tokens, cmd, &argc);
    cmd->argv[argc] = NULL;
    if (!(*cmd_list))
        *cmd_list = cmd;
    else
        (*last_cmd)->next = cmd;
    *last_cmd = cmd;
    if (tokens && tokens->type == TOKEN_PIPE)
        tokens = tokens->next;
    return tokens;
}

t_parser *parse_tokens(t_token *tokens)
{
    t_parser *cmd_list;
    t_parser *last_cmd;

	cmd_list = NULL;
	last_cmd = NULL;
    while (tokens && tokens->type != TOKEN_EOF)
    {
        while (tokens && tokens->type == TOKEN_PIPE)
        {
            printf("bash: syntax error near unexpected token `|'\n");
            tokens = tokens->next;
        }
        if (!tokens || tokens->type == TOKEN_EOF)
            break;

        tokens = ft_control_token(tokens, &cmd_list, &last_cmd);
    }
    return cmd_list;
}





void print_cmds(t_parser *cmd_list)
{
    int a = 1;
    while (cmd_list != NULL)
    {
        printf("Komut %d:\n", a);
        printf("  Komut ve Argümanlar:");
        int i = 0;
        while (cmd_list->argv && cmd_list->argv[i])
        {
            printf(" %s", cmd_list->argv[i]);
            i++;
        }
        if (!cmd_list->argv || !cmd_list->argv[0])
            printf(" (yok)");
        printf("\n");

        t_redirection *redir = cmd_list->redirs;
        while (redir)
        {
            if (redir->type == REDIR_IN)
            {
                printf("  < Redirection: %s\n", redir->filename);
            }
            else if (redir->type == REDIR_OUT)
            {
                printf("  > Redirection: %s (Overwrite)\n", redir->filename);
            }
            else if (redir->type == REDIR_APPEND)
            {
                printf("  >> Redirection: %s (Append)\n", redir->filename);
            }
            else if (redir->type == REDIR_HEREDOC)
            {
                printf("  << Heredoc: %s\n", redir->filename);
            }
            redir = redir->next;
        }

        printf("\n");
        cmd_list = cmd_list->next;
        a++;
    }
}

void print(t_parser *c)
{
    t_parser *current_cmd = c;
    int a = 0;
    while (current_cmd != NULL)
    {
        if (current_cmd->argv && current_cmd->argv[0] != NULL)
        {
            printf("%d ",a);
            int i = 0;
            while (current_cmd->argv[i] != NULL)
            {
                printf("'%s' ", current_cmd->argv[i]);
                i++;
            }
			// başarılı
			printf("(%s) ",current_cmd->argv[i]);
            printf("\n");
        }
        else
        {
            printf("Bu komutun argümanı yok.\n");
        }
        current_cmd = current_cmd->next;
        a++;
    }
}