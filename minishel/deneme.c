#include "minishell.h"

// void builtin_cd(t_token *args)
// {
//     const char *target_dir;

//     if (!args || !args->next || !args->next->value)
//     {
//         // Argüman yoksa HOME dizinine git
//         target_dir = getenv("HOME");
//         if (!target_dir)
//         {
//             printf("cd: HOME not set\n");
//             return;
//         }
//     }
//     else
//     {
//         target_dir = args->next->value;
//     }

//     if (chdir(target_dir) != 0)
//     {
//         perror("cd");
//     }
// }


char **tokenize(const char *input)
{
    int len = strlen(input);
    int i = 0;
    int token_count = 0;
    char **tokens = (char **)malloc(sizeof(char *) * (len / 2 + 2));
    if (!tokens)
        return NULL;

    while (i < len)
    {
        while (i < len && is_space(input[i]))
            i++;
        if (i >= len)
            break;

        int start = i;
        int token_len = 0;

        if (input[i] == '"' || input[i] == '\'')
        {
            char quote_char = input[i];
            i++;
            start = i;
            while (i < len && input[i] != quote_char)
                i++;
            token_len = i - start;
            if (i < len)
                i++;
        }
        else
        {
            start = i;
            while (i < len && !is_space(input[i]))
                i++;
            token_len = i - start;
        }

        char *token = (char *)malloc(token_len + 1);
        if (!token)
        {
            for (int k = 0; k < token_count; k++)
                free(tokens[k]);
            free(tokens);
            return NULL;
        }
        strncpy(token, input + start, token_len);
        token[token_len] = '\0';
        tokens[token_count++] = token;
    }

    tokens[token_count] = NULL;
    return tokens;
}

void free_tokens(char **tokens)
{
    if (!tokens)
        return;
    for (int i = 0; tokens[i] != NULL; i++)
        free(tokens[i]);
    free(tokens);
}

char *ft_lex_2(char *av, char *src)
{
    int i = 0;
    int a = 0;
    char value = 0;

    while (av[i])
    {
        if (value == 0 && (av[i] == '\'' || av[i] == '\"'))
        {
            value = av[i];
            i++;
        }
        else if (value != 0 && av[i] == value)
        {
            value = 0;
            i++;
        }
        else
        {
            src[a++] = av[i++];
        }
    }

    if (value != 0)
    {
        printf("Hata: Açık %c tırnağı kapatılmamış.\n", value);
        return NULL;
    }

    src[a] = '\0';
    return src;
}

void builtin_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        printf("getcwd hatası\n");
}

static char *my_history[MAX_HISTORY];
static int history_count = 0;

void add_command_to_history(const char *cmd)
{
    if (cmd && *cmd)
    {
        add_history(cmd);
        if (history_count < MAX_HISTORY)
        {
            my_history[history_count++] = strdup(cmd);
        }
        else
        {
            free(my_history[0]);
            for (int i = 1; i < MAX_HISTORY; i++)
                my_history[i - 1] = my_history[i];
            my_history[MAX_HISTORY - 1] = strdup(cmd);
        }
    }
}

void print_history(void)
{
    for (int i = 0; i < history_count; i++)
        printf("%d %s\n", i + 1, my_history[i]);
}

void free_history(void)
{
    for (int i = 0; i < history_count; i++)
        free(my_history[i]);
    history_count = 0;
}

// Linked list yapısı
typedef struct s_token
{
    char *value;
    struct s_token *next;
} t_token;

t_token *create_token(const char *val)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return NULL;
    new->value = strdup(val);
    new->next = NULL;
    return new;
}

void add_token(t_token **head, const char *val)
{
    t_token *new = create_token(val);
    if (!new)
        return;

    if (!*head)
        *head = new;
    else
    {
        t_token *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void print_tokens_list(t_token *head)
{
    int i = 0;
    while (head)
    {
        printf("Token[%d]: %s\n", i++, head->value);
        head = head->next;
    }
}

void free_tokens_list(t_token *head)
{
    t_token *tmp;
    while (head)
    {
		tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}

int ctrl(char *av)
{
	int i = 0;
	while(av[i])
	{
		if (av[i] != '|')
			i++;
		else 
			return (0);
	}
	return (1);
}
int nl_control(t_token *av)
{
    // Öncelikle av'nin ve av->value'nin NULL olmadığını kontrol et
    if (av == NULL || av->value == NULL)
    {
        return (0); // Geçersiz argüman, 0 döndür
    }

    // Eğer av->value'nin uzunluğu en az 2 ise (örn: "-n") ve
    // ilk karakteri '-' ise
    if (strlen(av->value) >= 2 && av->value[0] == '-')
    {
        // Eğer ikinci karakter 'n' ise (yani "-n" ise)
        // ** Önemli: pointer'ı ilerletmiyoruz, sadece indeksi kullanıyoruz **
        if (av->value[1] == 'n')
        {
            // Eğer tam olarak "-n" stringini kontrol etmek istiyorsanız, strcmp kullanın
            // if (strcmp(av->value, "-n") == 0) // "-n" dışındaki "-nn", "-nsomething" gibi durumları engeller
// Debug için
            return (1); // 'n' bayrağı bulundu
        }
        // Eğer ilk karakter '-' ama ikinci karakter 'n' değilse
        else
        {
            // Debug için basabilirsiniz, bu kısmı ihtiyacınıza göre düzenleyin
            // printf("Geçersiz bayrak: %c\n", av->value[1]);
            return (0); // Geçersiz bayrak
        }
    }
    // Eğer av->value '-' ile başlamıyorsa veya çok kısaysa
    return (0); // Geçersiz durum
}

int main()
{
    char *line;
    char **args;
    char *res = malloc(sizeof(char) * 100);
    read_history(".minishell_history");

    while (1)
    {
        line = readline("minishell-> ");
        if (!line)
            break;

        if (line[0] != '\0')
            add_command_to_history(line);

        args = tokenize(line);
        if (!args || !args[0])
        {
            free(line);
            break;
        }

        t_token *tokens_list = NULL;
        for (int i = 0; args[i]; i++)
        {
            char cleaned[100];
            ft_lex_2(args[i], cleaned);
            add_token(&tokens_list, cleaned);
        }
        if (tokens_list && my_strcmp(tokens_list->value, "echo") == 0)
        {
            t_token *tmp = tokens_list->next;
            t_token *res = tokens_list->next;
			if(nl_control(res))
            {
				tmp = tmp->next;
			}
            while (tmp)
            {
				if (ctrl(tmp->value))
				{
					
					printf("%s ", tmp->value);
					tmp = tmp->next;
				}
				else 
				break;
            }
			if(!nl_control(res))
            	printf("\n");
        }
        else if (tokens_list && my_strcmp(tokens_list->value, "pwd") == 0)
        {
            builtin_pwd();
        }
        else if (tokens_list && my_strcmp(tokens_list->value, "history") == 0)
        {
            print_history();
        }
        else if (tokens_list && my_strcmp(tokens_list->value, "exit") == 0)
        {
            free(line);
            break;
        }
        else
        {
            printf("Komut: %s\n", tokens_list->value);
            t_token *tmp = tokens_list->next;
            int i = 1;
            while (tmp)
            {
                printf("Arg[%d]: %s\n", i++, tmp->value);
                tmp = tmp->next;
            }
        }

        free_tokens_list(tokens_list);
        free_tokens(args);
        free(line);
    }

    write_history(".minishell_history");
    free_history();
    free(res);
    return 0;
}
