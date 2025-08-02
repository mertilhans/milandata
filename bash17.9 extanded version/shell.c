/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 03:36:25 by husarpka          #+#    #+#             */
/*   Updated: 2025/08/02 07:33:38 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_tokenizer *tokenizer_init(char *input)
{
    t_tokenizer *tokenizer;
    
    if (!input)
    return (NULL);
    
    tokenizer = gb_malloc(sizeof(t_tokenizer));
    if (!tokenizer)
    return (NULL);
    
    tokenizer->input = ft_strdup(input);
    if (!tokenizer->input)
    {
        gc_free(tokenizer);
        return (NULL);
    }
    
    tokenizer->pos = 0;
    tokenizer->len = ft_strlen(input);
    
    if(tokenizer->len > 0)
    tokenizer->current = input[0];
    else
    tokenizer->current = '\0';  
    return (tokenizer);
}

// token oluştur  tip ve değer ile
t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = gb_malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    if (value)
        token->value = ft_strdup(value);
    else
        token->value = NULL;
    token->next = NULL;
    return (token);
}


void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    const char *type_names[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND",
        "HEREDOC", "AND", "OR", "SEMICOLON", "LPAREN", "RPAREN",
        "EOF", "ERROR"
    };
    
    printf("=== TOKENS ===\n");
    while (current)
    {
        char *value_str;
        if (current->value)
            value_str = current->value;
        else
            value_str = "NULL";
        
        printf("Type: %-12s Value: '%s'\n", 
               type_names[current->type], 
               value_str);
        current = current->next;
    }
    printf("==============\n");
}

int red_len(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

int ft_ctrl(char *str)
{
    int i = 0;
    if (str[i] && str[i + 1])
    {
        if (str[i] == '>' && str[i + 1] == '<')
            i++;
    }
    while(str[i] == '<')
        i++;
    if(str[i] == '\0')
        return (1);
    else
        return (0);
}

int ft_ctrl_2(char *str)
{
    int i = 0;
    if (str[i] && str[i + 1])
    {
        if (str[i] == '<' && str[i + 1] == '>')
            i = 1;
    }
    else
        return (i);
    return (i);
}
int ft_ctr_3(char *str)
{
	int i = 0;
	str++;
	str++;
	if (str[i] == '<' && str[i + 1] == '>')
		return (1);
	else 
		return (i);
	return (0);
}

int ft_alphabet(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	else if (c >= 'A'  && c <= 'Z')
		return (1);
	else 
		return (0);
}

static t_export *g_export_list = NULL;

t_export **get_export_list(void)
{
    return &g_export_list;
}

int main(int ac , char **av, char **env)
{
	(void)ac;
	(void)av;
	
	// Initialize environment variables
	t_env *env_list = init_env(env);
	
	// Initialize export list from env
	g_export_list = init_export_from_env(env_list);
	
	// Set SHLVL - increment shell level
	t_env *shlvl_node = find_env(env_list, "SHLVL");
	int shlvl_value = 1; // Default to 1 for minishell
	if (shlvl_node && shlvl_node->value)
		shlvl_value = atoi(shlvl_node->value) + 1;
	
	char *shlvl_str = ft_itoa(shlvl_value);
	set_env_value(&env_list, "SHLVL", shlvl_str);
	set_export_value(&g_export_list, "SHLVL", shlvl_str);
	
	// Setup signal handlers
	setup_interactive_signals();
	
	char *line;
	t_token *tokens;
	int last_exit_status = 0;
	
	while(1)
	{
		// BUNU bölücez anladığım kadarıyla child processlerde kullanmak için
		check_and_handle_signal(&last_exit_status);
		
		line = readline("MiniShell->>>   ");
		if (!line)
		{
			// EOF (Ctrl+D) veya hata durumu
			printf("exit\n");
			break;
		}
		
		if (line[0] == '\0')
			gc_free(line);
		else
		{
			add_history(line);
			tokens = tokenize_input(line);
			if(tokens)
			{

				t_parser *cmd_list = parse_tokens(tokens, env_list, last_exit_status);
				
				char **updated_env = env_list_to_array(env_list);
				if (updated_env)
				{
					execute_cmds(cmd_list, updated_env, &env_list);
					int i = 0;
					while (updated_env[i])
					{
						gc_free(updated_env[i]);
						i++;
					}
					gc_free(updated_env);
				}
				else
					execute_cmds(cmd_list, env, &env_list);
			}
			else
			{
				// Tokenization başarısızsa programı sonlandırma, sadece hata ver
				if (line && ft_strlen(line) > 0)
					printf("tokenization failed\n");
			}
		}
		
		// readline dan dönen değer yukarı da da yapabiliriz ama burada da yapalım
		gc_free(line);
	}
    
    return (0);
}