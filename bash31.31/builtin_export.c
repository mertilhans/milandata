#include "shell.h"

// export listesinin hepsini yazdır abi
void export_print_all(t_export **export_list)
{
    t_export *current = *export_list;
    while (current)
    {
        printf("declare -x %s", current->key);
        if (current->value)
            printf("=\"%s\"", current->value);
        printf("\n");
        current = current->next;
    }
}

// değer birleştirme işlemi - export TEST="hello world" gibiler için
char *export_build_value(t_parser *cmd, int *i, char *value)
{
    char *full_value;
    int j;
    
    full_value = gb_malloc(1024);
    strcpy(full_value, value);
    j = *i + 1;
    while (cmd->argv[j] && !strchr(cmd->argv[j], '='))
    {
        strcat(full_value, " ");
        strcat(full_value, cmd->argv[j]);
        j++;
    }
    
    *i = j - 1;
    return full_value;
}

// key=value formatındaki argumentları işle reis
void export_process_keyvalue(t_parser *cmd, int *i, t_env **env_list)
{
    t_export **export_list;
    char *current_arg;
    char *eq_pos;
    char *key;
    char *value;
    
    export_list = get_export_list();
    current_arg = cmd->argv[*i];
    eq_pos = strchr(current_arg, '=');
    
    *eq_pos = '\0';
    key = current_arg;
    value = eq_pos + 1;
    
    if (cmd->argv[*i + 1] && ft_strlen(value) > 0)
    {
        char *full_value = export_build_value(cmd, i, value);
        set_export_value(export_list, key, full_value);
        set_env_value(env_list, key, full_value);
        free(full_value);
    }
    else
    {
        set_export_value(export_list, key, value);
        set_env_value(env_list, key, value);
    }
    
    *eq_pos = '=';
}

// ana export fonksiyonu - bu arkadaş çok iş yapıyor
void builtin_export(t_parser *cmd, t_env **env_list)
{
    int i;
    t_export **export_list;
    
    i = 1;
    export_list = get_export_list();
    
    if (!cmd->argv[1])
    {
        export_print_all(export_list);
        return;
    }
    
    while (cmd->argv[i])
    {
        char *current_arg;
        char *eq_pos;
        
        current_arg = cmd->argv[i];
        eq_pos = strchr(current_arg, '=');
        
        if (eq_pos)
            export_process_keyvalue(cmd, &i, env_list);
        else
            set_export_value(export_list, cmd->argv[i], NULL); // sadece key, value yok
        i++;
    }
}

// değişkeni hem env hem export listesinden kaldır abi
void builtin_unset(t_parser *cmd, t_env **env_list)
{
    int i;
    t_export **export_list;
    
    i = 1;
    export_list = get_export_list();
    
    if (!cmd->argv[1])
    {
        fprintf(stderr, "unset: not enough arguments\n");
        return;
    }
    
    while (cmd->argv[i])
    {
        unset_env_value(env_list, cmd->argv[i]);
        unset_export_value(export_list, cmd->argv[i]);
        i++;
    }
}
