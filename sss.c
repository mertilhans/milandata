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
    else if (str[i] == '>')
        return (2);
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
int ft_ctrl_4(char *str)
{
   int i = 0;
    if (str[i] && str[i + 1])
    {
        if (str[i] == '>' && str[i + 1] == '<')
            i = 1;
    }
    else
        return (i);
    return (i);
 }

int ctrrr(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    if (str[i] == '\0')
        return(i);
    return (0);
    
} 
void error(char *str)
{
    int i;
    int a = 0;
    int value;
    int c = 0;

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
    else if (ctrrr(str) > 2)
    {
        c = ctrrr(str);
        if (c == 3)
            printf("bash: syntax error near unexpected token `>'\n");
        else if (c >= 4)
            printf("bash: syntax error near unexpected token `>>'\n");
            
    }
    else if (res[i] == '>' && res[i + 1] == '<')
    {
        a = red_len(str);
       
        value = ft_ctrl(str);
        printf("(%d)",value);
        if (res[i] == '>' && res[i + 1] == '<' && res[i + 2] == '>')
            printf("bash: syntax error near unexpected token `<>'");
       else if (value == 1)
        {
            printf("bash: syntax error near unexpected token `");
            if (a > 3)
                a = 4;
            while(--a)
                printf("<");
            printf("'");
        }
        else if (ft_ctrl_2(str))
        {

           printf("bash: syntax error near unexpected token `<>'");
           
        }
        printf("\n");
    }
        //printf("bash: syntax error near unexpected token `<'\n");
    else if (res[i] == '<' && res[i + 1] == '>' && res[i + 2] == '\0')
        printf("bash: syntax error near unexpected token `newline'\n");
    else if (res[i] == '<' && res[i + 1] == '>' )
	{
		if (ft_ctr_3(str))
			printf("bash: syntax error near unexpected token `<>'\n");
		else
        	printf("bash: syntax error near unexpected token `");
            a = red_len(str);
            if (a > 3)
                a = 4;
            while(--a)
                printf(">");
           printf("'");
                
           printf("\n");
		
	}
	else if (res[i] == '<' && ft_alphabet(res[i + 1]))
	{
        res++;
		printf("bash: %s: No such file or directory\n",res);
	}
    else if (res[i] == '<' || res[i] == '>')
        printf("bash: syntax error near unexpected token `newline'\n");
    ft_error_2(res);
	    //printf("%s \n",res);
        
}
