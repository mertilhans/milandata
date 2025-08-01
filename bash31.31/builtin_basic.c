#include "shell.h"

// echo komutu - -n flag'i de destekliyor
void built_echo(t_parser *cmd)
{
    int i;
    int newline;
    
    i = 1;
    newline = 1;
    
    if (cmd->argv[1] && cmd->argv[1][0] == '-' && cmd->argv[1][1] == 'n')
    {
        // "-n den sonra ki kontrol aq kontrolü
        int j = 2;
        int valid_flag = 1;
        while (cmd->argv[1][j])
        {
            if (cmd->argv[1][j] != 'n')
            {
                valid_flag = 0;
                break;
            }
            j++;
        }
        
        if (valid_flag)
        {
            newline = 0; // -n flag varsa newline ekleme
            i = 2; 
        }
    }
    
    while(cmd->argv[i])
    {
        printf("%s", cmd->argv[i]);
        if(cmd->argv[i + 1] != NULL)
            printf(" ");
        i++;
    }
    
    if (newline)
        printf("\n");
}

// cd komutu - directory değiştir
void built_cd(t_parser *cmd)
{
    if (cmd->argv[1] == NULL) {
        fprintf(stderr, "cd: argument missing\n");
        return;
    }
    if (chdir(cmd->argv[1]) != 0)
    {
        perror("cd");
        return;
    }
}

// pwd komutu - şu anki dizini göster
void builtin_pwd(void)
{
    char *cwd = getcwd(NULL, 0);
    if (cwd)
    {
        printf("%s\n", cwd);
        free(cwd);
    }
    else
        perror("pwd");
}

// exit komutu - shell'den çık
void builtin_exit(t_parser *cmd)
{
    int exit_code;
    
    exit_code = 0;
    
    if (cmd->argv[1])
        exit_code = ft_atoi(cmd->argv[1]);
    printf("exit\n");
    exit(exit_code);
}

// env komutu - environment değişkenlerini listele
void builtin_env(t_env *env_list)
{
    t_env *current = env_list;
    
    while (current)
    {
        if (current->value)
            printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}
