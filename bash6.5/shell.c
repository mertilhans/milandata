/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 03:36:25 by husarpka          #+#    #+#             */
/*   Updated: 2025/07/20 08:03:28 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_tokenizer *tokenizer_init(char *input)
{
    t_tokenizer *tokenizer;
    
    if (!input)
    return (NULL);
    
    tokenizer = malloc(sizeof(t_tokenizer));
    if (!tokenizer)
    return (NULL);
    
    tokenizer->input = ft_strdup(input);
    if (!tokenizer->input)
    {
        free(tokenizer);
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

t_token *create_token(t_token_type type, char *value)
{
    t_token *token;
    
    token = malloc(sizeof(t_token));
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
        printf("Type: %-12s Value: '%s'\n", 
               type_names[current->type], 
               current->value ? current->value : "NULL");
        current = current->next;
    }
    printf("==============\n");
}
void ft_error_syntax(t_token *token)
{
    printf("---tokenlerrrrrrrr(%c)---",token->value[0]);
    printf("---tokenlerrrrrrrr(%c)---",token->value[1]);
    printf("---tokenlerrrrrrrr(%c)---",token->value[2]);

    
    printf("token 1 %c \n token 2%c \n",token->value[0],token->value[1]);
    if (token->value[0] == '|')
        printf("bash: syntax error near unexpected token `|'\n");
    else if (token->value[0] == '>' && token->value[1] == '<')
        printf("bash: syntax error near unexpected token `<'\n");
    else if (token->value[0] == '<' && token->value[1] == '>')
        printf("bash: syntax error near unexpected token `newline'\n");
    else if (token->value[0] == '>' || token->value[0] == '<')
        printf("bash: syntax error near unexpected token `newline'\n");
        
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
void ft_error_2(char *str)
{
	int i = 0;
	if (str[i] == '~')
		printf("bash: /home/husarpka: Is a directory\n");
}

void error(char *str)
{
    int i;
    int a = 0;

    i = 0;
    char *res = ft_strdup(str);
    while(res)
    {
        if (*res == 32)
            res++;
        else 
            break;
    }
    if (res[i] == '|')
        printf("bash: syntax error near unexpected token `|'\n");
    else if (res[i] == '>' && res[i + 1] == '<')
    {
        a = red_len(str);
        if (ft_ctrl(str))
        {
            printf("bash: syntax error near unexpected token `");
            if (a > 3)
                a = 4;
            while(--a)
                printf("<");
        }
        str++;
        if (ft_ctrl_2(str))
        {

           printf("bash: syntax error near unexpected token `<>'");
           
        }
    
    
        printf("\n");
        
        
        
    }
        //printf("bash: syntax error near unexpected token `<'\n");
    else if (res[i] == '<' && res[i + 1] == '>' )
	{
		if (ft_ctr_3(str))
			printf("bash: syntax error near unexpected token `<>'\n");
		else
        	printf("bash: syntax error near unexpected token `newline'\n");
		
	}
	else if (res[i] == '<' && ft_alphabet(res[i + 1]))
	{
		printf("bash: %c: No such file or directory\n",res[i + 1]);
	}
    else if (res[i] == '<' || res[i] == '>')
        printf("bash: syntax error near unexpected token `newline'\n");
    ft_error_2(res);
	    //printf("%s \n",res);
        
}



int main(int ac , char **av, char **env)
{
	(void)ac;
	(void)av;
	//char *envr = ft_env(env);
	char *line;
	t_token *tokens;
	
	while(1)
	{
		line = readline("MiniShell->>>   ");
        add_history(line);
		tokens = tokenize_input(line);
		if(tokens)
		{
			//print_tokens(tokens);
            error(line);
            //ft_error_syntax(tokens);
		    //token_list_free(tokens);
			t_parser *cmd_list = parse_tokens(tokens);
			//print(cmd_list);
			//print_cmds(cmd_list);
			//printf("(%d) ", count_commands(cmd_list));
			
			execute_cmds(cmd_list, env);
			//free_parser_list(cmd_list);
			
			
		}
		else
		{
			
			printf("tokenization failed\n");
			exit(0);
		}
	}
    
    return (0);

}