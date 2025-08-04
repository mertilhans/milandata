#include "shell.h"

// echo komutu - -n flag'i de destekliyor
void built_echo(t_parser *cmd)
{
    int i;
    int newline;
    
    i = 1;
    newline = 1;
    
    // Tüm -n flaglerini kontrol et (bash davranışı)
    while (cmd->argv[i] && cmd->argv[i][0] == '-' && cmd->argv[i][1] == 'n')
    {
        int j = 2;
        int valid_flag = 1;
        
        while (cmd->argv[i][j])
        {
            if (cmd->argv[i][j] != 'n')
            {
                valid_flag = 0;
                break;
            }
            j++;
        }
        
        if (valid_flag)
        {
            newline = 0; 
            i++; 
        }
        else
            break; 
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

void built_cd(t_parser *cmd)
{
    if (cmd->argv[1] == NULL) {
        printf("cd: argument missing\n");
        return;
    }
	if (cmd->argv[2])
		printf("bash: cd: too many arguments\n");
    if (chdir(cmd->argv[1]) != 0)
    {
        perror("cd");
        return;
    }
}

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

void builtin_exit(t_parser *cmd)
{
    int exit_code;
    
    exit_code = 0;
    
    if (cmd->argv[1])
        exit_code = ft_atoi(cmd->argv[1]);

    exit(exit_code);
}

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
