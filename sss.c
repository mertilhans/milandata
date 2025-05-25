#include "minishell.h"
#include "stdio.h"
#include "unistd.h"
# include <readline/readline.h>






int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    if (!s1 || !s2) 
        return -1; 

    while (s1[i] && s2[i] && s1[i] == s2[i])
    {
        i++;
    }
    return (s1[i] - s2[i]);
}

//int ft_lexer_echo(char *av)
//{
  //  int i = 0;
    //if (av[i] == 'e')
    
    //return (0);
//}
char *control(char *av)
{
    int i = 0;
    int j = 0; 
    char *res = malloc(sizeof(char) * 1000);
    if (!res)
        return NULL;

    while (av[i] != '\0')
    {
        
        if (av[i] != '\'') 
        {
            res[j] = av[i];
            j++;
        }
        i++;
    }
    return (res);
}

int ft_control_2(char *av)
{
    int i = 0;
    int a = 0;
    while(av[i])
    {
        if (av[i] == '\"')
            a++;
        i++;
    }
    if (a % 2 == 0)
        return (1);
    return (0);

}

char *ft_control_4(char *av)
{
    int i = 0;
    int j = 0; 
    char *res = malloc(sizeof(char) * 1000);
    if (!res)
        return NULL;

    while (av[i] != '\0')
    {
        
        if (av[i] != '\"') 
        {
            res[j] = av[i];
            j++;
        }
        i++;
    }
    return (res);
}

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);

}

int ft_echo(char *av)
{
        printf("%s ",av);
    return (0);
}

// int ft_cat_handle(char *av)
// {

//     (void)av;
//     int fd; 
//     int read_size;
//     char buffer[BUFFER_SIZE + 1];
//     const char *filename = av;


//     fd = open(filename,O_RDONLY,777);
//     if (fd == -1)
//         printf("hata");
//     while ((read_size = read(fd,buffer,BUFFER_SIZE)) > 0)
//     {
//         buffer[read_size] = '\0';
//         write(1,buffer,read_size);
//     }
//     close(fd);
//     return (0);
// }


// int ft_cat_control(char *av)
// {
//     if (!(ft_strcmp("cat",av)))
//     {
//         printf("başarili\n");
//      return (1);

//     }
//     return (0);
// }
int lexer(char *av)
{
    // printf("sarp\n");
    // if (ft_cat_control(av))
    //     return (3);
    if (!(ft_control_2(av)))
        return (0);
    av = ft_control_4(av);
    char *res = control(av);
    if (!ft_strcmp("echo",res))
        return (1);
    
    // ft_echo(av);
    return (1);
}
char *ft_space(char *str)
{
    int i = 0;
    int a = 0;
    char *res;
    res = malloc(sizeof(char) * ft_strlen(str));

    while(str[i])
    {
        if (str[i] != 32)
        i++;
        else
        break;
    }
    i++;
    if(!str[i])
    {
        write(1,"\n",1);
        return (NULL);
    }
    while(str[i])
    {
        res[a] = str[i];
        a++;
        i++;
    }
    res[a] = '\0';
    return(res);
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    char *res;
    char *value;
    value = readline("Minishell-> ");
    res = value;
    
    res = ft_space(res);
    pid_t pid = fork();
    if (pid == 0) 
    {
        if(lexer(value) == 1)
        {
            
            if(res == NULL)
            return 0;
            printf("%s \n",res);
            
        }
        // if (lexer(value) == 3)
        // {
        //     printf("sarp");
        //     // printf("%s ",res);
        //     // exit(0);
        //     ft_cat_handle(res);
        //     printf("cat başarili\n");
        // }
    
    } else if (pid > 0) {
        wait(NULL); 
        
    } else {
        perror("fork");
    }
    free(res); 
    return 0;
}
