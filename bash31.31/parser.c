#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

// parser yapısını temizle ve başlat abi
void ft_clean_init(t_parser *cmd)
{
    cmd->argv_cap = 1;
    cmd->argv = gb_malloc(sizeof(char*) * cmd->argv_cap);
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

// redirection ekle (<, >, >>, << işlemleri)
void add_redirection(t_parser *cmd, t_redir_type type, char *filename)
{
    t_redirection *new_redir = gb_malloc(sizeof(t_redirection));
    if (!new_redir)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_redir->filename = ft_strdup(filename); 
    if (!new_redir->filename)
    {
        perror("strdup failed for redirection filename");
        free(new_redir);
        exit(EXIT_FAILURE);
    }
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

void ft_redir_in(t_token *tokens, t_parser *cmd, t_env *env_list, int exit_status)
{
    tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
    {
        char *expanded = expand_with_quotes(tokens->value, env_list, exit_status);
        add_redirection(cmd, REDIR_IN, expanded ? expanded : tokens->value);
        if (expanded && expanded != tokens->value)
            free(expanded);
    }
    else
        cmd->parse_error = 1;
}

void ft_redir_out(t_token *tokens, t_parser *cmd, t_env *env_list, int exit_status)
{
    tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
    {
        char *expanded = expand_with_quotes(tokens->value, env_list, exit_status);
        add_redirection(cmd, REDIR_OUT, expanded ? expanded : tokens->value);
        if (expanded && expanded != tokens->value)
            free(expanded);
    }
    else
        cmd->parse_error = 1;
}

void ft_redir_append(t_token *tokens, t_parser *cmd, t_env *env_list, int exit_status)
{
    tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
    {
        char *expanded = expand_with_quotes(tokens->value, env_list, exit_status);
        add_redirection(cmd, REDIR_APPEND, expanded ? expanded : tokens->value);
        if (expanded && expanded != tokens->value)
            free(expanded);
    }
    else
        cmd->parse_error = 1;
}

void ft_redir_heredoc(t_token *tokens, t_parser *cmd, t_env *env_list, int exit_status)
{
    (void)env_list;  // For heredoc, we usually don't expand the delimiter
    (void)exit_status;  // Not needed for heredoc delimiter
    tokens = tokens->next;
    if (tokens && tokens->type == TOKEN_WORD)
    {
        add_redirection(cmd, REDIR_HEREDOC, tokens->value);
    }
    else
        cmd->parse_error = 1;
}

void ft_loop_3(t_token **tokens, t_parser *cmd, int *argc, t_env *env_list, int exit_status)
{
    int i;
    char **new_argv;

    i = 0;
    if ((*tokens)->type == TOKEN_WORD)
    {
        if (*argc + 1 >= cmd->argv_cap)
        {
            cmd->argv_cap *= 2;
            new_argv = gb_malloc(sizeof(char*) * cmd->argv_cap);
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
        // Apply variable expansion
        char *expanded = expand_with_quotes((*tokens)->value, env_list, exit_status);
        
        // If expansion happened and it contains spaces, split it
        if (expanded && expanded != (*tokens)->value && 
            (strchr(expanded, ' ') || strchr(expanded, '\t')))
        {
            char **split_args = split_expanded_string(expanded);
            int j = 0;
            
            while (split_args && split_args[j])
            {
                if (*argc + 1 >= cmd->argv_cap)
                {
                    cmd->argv_cap *= 2;
                    char **new_argv = gb_malloc(sizeof(char*) * cmd->argv_cap);
                    if (!new_argv)
                    {
                        perror("malloc failed");
                        exit(EXIT_FAILURE);
                    }
                    int k = 0;
                    while(k < *argc)
                    {
                        new_argv[k] = cmd->argv[k];
                        k++;
                    }
                    free(cmd->argv);
                    cmd->argv = new_argv;
                }
                cmd->argv[(*argc)++] = ft_strdup(split_args[j]);
                j++;
            }
            free(expanded);
        }
        else
        {
            cmd->argv[(*argc)++] = expanded ? expanded : ft_strdup((*tokens)->value);
        }
        
        if (!cmd->argv[(*argc) - 1])
        {
            perror("strdup failed for argv element");
            exit(EXIT_FAILURE);
        }
    }
}

void ft_loop_2(t_token **tokens, t_parser *cmd,int *argc, t_env *env_list, int exit_status)
{
    if ((*tokens)->type == TOKEN_REDIR_APPEND)
    {
        ft_redir_append(*tokens, cmd, env_list, exit_status);
        if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
            *tokens = (*tokens)->next;
    }
    else if ((*tokens)->type == TOKEN_HEREDOC)
    {
        ft_redir_heredoc(*tokens, cmd, env_list, exit_status);
        if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
            *tokens = (*tokens)->next;
    }
    else
        ft_loop_3(tokens,cmd,argc, env_list, exit_status);

}

void ft_loop(t_token **tokens, t_parser *cmd, int *argc, t_env *env_list, int exit_status)
{
    if (cmd->parse_error)
        printf("bash: syntax error near unexpected token `<'\n"); 

    if ((*tokens)->type == TOKEN_REDIR_IN)
    {
        ft_redir_in(*tokens, cmd, env_list, exit_status);
        if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
            *tokens = (*tokens)->next;
    }
    else if ((*tokens)->type == TOKEN_REDIR_OUT)
    {
        ft_redir_out(*tokens, cmd, env_list, exit_status);
        if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
            *tokens = (*tokens)->next;
    }
    else
        ft_loop_2(tokens,cmd,argc, env_list, exit_status);

    *tokens = (*tokens)->next;
}

t_token *ft_control_token(t_token *tokens, t_parser **cmd_list, t_parser **last_cmd, t_env *env_list, int exit_status)
{
    int argc;
    t_parser *cmd;

    argc = 0;
    cmd = gb_malloc(sizeof(t_parser));
    if (!cmd)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    ft_clean_init(cmd);
    while (tokens && tokens->type != TOKEN_PIPE && tokens->type != TOKEN_EOF)
        ft_loop(&tokens, cmd, &argc, env_list, exit_status);
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

t_parser *parse_tokens(t_token *tokens, t_env *env_list, int exit_status)
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

        tokens = ft_control_token(tokens, &cmd_list, &last_cmd, env_list, exit_status);
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
