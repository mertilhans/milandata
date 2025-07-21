#include "shell.h"

// const   char *builtin_command[] = 
// {
//         "echo",
//         "cd",
//         "pwd",
//         "exit",
//         NULL
// };

// int     is_builtin(char *av)
// {
//         int i = 0;
//         while(builtin_command[i])
//         {
//                 if(strcmp(av,builtin_command[i]) == 0)
//                         return i;
//         }
//         return -1;
// }
int     built_echo(t_parser *cmd)
{
        int i = 1;
        while(cmd->argv[i])
        {
                printf("%s",cmd->argv[i]);
                        if(cmd->argv[i + 1] != NULL)
                                printf(" ");
                        i++;
        }
        printf("\n");
        return 0;
}
int built_cd(t_parser *cmd)
{
    // Eğer argüman yoksa
	printf("0 = %s \n",cmd->argv[0]);
	printf("1 = %s \n",cmd->argv[1]);

    if (cmd->argv[1] == NULL) {
        fprintf(stderr, "cd: argument missing\n");
        return 1;  // Hata kodu
    }
    if (chdir(cmd->argv[1]) != 0)
	{
        perror("cd");
        return 1;
    }
    return 0;
}
int     builtin_pwd(void)
{
	
        char cwd[1024]; // env ile alakali // never forget this env
        if(getcwd(cwd,sizeof(cwd)) != NULL)
        {
                printf("%s\n",cwd);
                return 0;
        }
        else
        {
                perror("pwd");
                return 1;
        }
}
int builtin_exit(t_parser *cmd)
{
        int code = 0; // çıkış kodu dedik default 
        if(cmd->argv[1] != NULL)
                code = atoi(cmd->argv[1]); // input olarak alıyoruz verirsek 
        printf("exit\n");
        exit(code);
        return 0;
}
// int run_builtin(char **av)
// {
//         int id;
//         id = is_builtin(av[0]);
//         if(id == -1)
//                 return -1;
//         if(id == 0)
//                 return (builtin_echo(av));
//         else if(id == 1)
//                 return (built_cd(av));
//         else if(id == 2)
//                 return (builtin_pwd(av));
//         else if (id == 3)
//                 return (builtin_exit(av));
//         else
//                 return -1
// }