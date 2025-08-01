#include "shell.h"

void built_echo(t_parser *cmd)
{
    int i = 1;
    int newline = 1; // Default: newline ekle
    
    // -n flag kontrolü
    if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-n") == 0)
    {
        newline = 0; // -n flag varsa newline ekleme
        i = 2; // Argumentlara 2. indeksten başla
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
        fprintf(stderr, "cd: argument missing\n");
        return;
    }
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
    int exit_code = 0;
    
    if (cmd->argv[1])
        exit_code = ft_atoi(cmd->argv[1]);
    printf("exit\n");
    exit(exit_code);
}

void builtin_export(t_parser *cmd, t_env **env_list)
{
    int i = 1;
    t_export **export_list = get_export_list();
    
    if (!cmd->argv[1])
    {
        // Export without arguments - print all exported variables
        t_export *current = *export_list;
        while (current)
        {
            printf("declare -x %s", current->key);
            if (current->value)
                printf("=\"%s\"", current->value);
            printf("\n");
            current = current->next;
        }
        return;
    }
    
    while (cmd->argv[i])
    {
        char *current_arg = cmd->argv[i];
        char *eq_pos = strchr(current_arg, '=');
        
        if (eq_pos)
        {
            // KEY=VALUE format - reconstruct full value from remaining args
            *eq_pos = '\0';
            char *key = current_arg;
            char *value = eq_pos + 1;
            
            // If value seems incomplete (no space and more args), concatenate
            if (cmd->argv[i + 1] && strlen(value) > 0)
            {
                // Build full value string
                char *full_value = gb_malloc(1024); // temp buffer
                strcpy(full_value, value);
                
                // Add remaining args until we hit next KEY= or end
                int j = i + 1;
                while (cmd->argv[j] && !strchr(cmd->argv[j], '='))
                {
                    strcat(full_value, " ");
                    strcat(full_value, cmd->argv[j]);
                    j++;
                }
                
                set_export_value(export_list, key, full_value);
                set_env_value(env_list, key, full_value);
                
                free(full_value);
                i = j - 1; // Skip processed args
            }
            else
            {
                set_export_value(export_list, key, value);
                set_env_value(env_list, key, value);
            }
            
            *eq_pos = '='; // restore
        }
        else
        {
            // Just KEY format (export a) - sadece export listesine ekle
            set_export_value(export_list, cmd->argv[i], NULL);
        }
        i++;
    }
}

void builtin_unset(t_parser *cmd, t_env **env_list)
{
    int i = 1;
    t_export **export_list = get_export_list();
    
    if (!cmd->argv[1])
    {
        fprintf(stderr, "unset: not enough arguments\n");
        return;
    }
    
    while (cmd->argv[i])
    {
        // Hem env hem export listesinden kaldır
        unset_env_value(env_list, cmd->argv[i]);
        unset_export_value(export_list, cmd->argv[i]);
        i++;
    }
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