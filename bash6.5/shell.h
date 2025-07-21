/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 03:36:35 by husarpka          #+#    #+#             */
/*   Updated: 2025/07/20 09:08:18 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
#include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <sys/wait.h>

// Token tipleri
typedef enum e_token_type
{
    TOKEN_WORD,          // Normal kelimeler
    TOKEN_PIPE,          // |
    TOKEN_REDIR_IN,      // <
    TOKEN_REDIR_OUT,     // >
    TOKEN_REDIR_APPEND,  // >>
    TOKEN_HEREDOC,       // <<
    TOKEN_AND,           // &&
    TOKEN_OR,            // ||
    TOKEN_SEMICOLON,     // ;
    TOKEN_LPAREN,        // (   
    TOKEN_RPAREN,        // )
    TOKEN_EOF,           // Dosya sonu
    TOKEN_ERROR          // Hata
} t_token_type;

// Token yapısı
typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
} t_token;

// Tokenizer yapısı
typedef struct s_tokenizer
{
    char    *input;      // Giriş metni
    int     pos;         // Mevcut pozisyon
    int     len;         // Toplam uzunluk
    char    current;     // Mevcut karakter
} t_tokenizer;

// --- YENİ EKLENEN/DEĞİŞEN YAPILAR ---

// Redirection türlerini tanımlayan enum
typedef enum e_redir_type
{
    REDIR_IN,       // <
    REDIR_OUT,      // >
    REDIR_APPEND,   // >>
    REDIR_HEREDOC   // <<
} t_redir_type;

// Her bir redirection için bilgi tutan yapı
typedef struct s_redirection
{
    char                *filename; // Yönlendirilecek dosya adı veya heredoc delimiter
    t_redir_type        type;      // Redirection tipi
    struct s_redirection *next;     // Bir sonraki redirection
} t_redirection;

// Komut yapısı (t_parser olarak adlandırılmış)
typedef struct s_parser
{
    char                **argv;         // Komut ve argümanlar
    int                 argv_cap;       // argv dizisinin kapasitesi
    t_redirection       *redirs;        // Redirection'lar için bağlı liste
    struct s_parser *next;          // Pipe için
    int                 parse_error;    // Ayrıştırma sırasında hata olup olmadığını belirtir (opsiyonel)
} t_parser;



// --- PARSER FONKSİYON PROTOTİPLERİ ---
t_parser *parse_tokens(t_token *tokens);
void print_cmds(t_parser *cmd_list); // Yeniden adlandırdım, `print` yerine daha açıklayıcı
void ft_add_redirection(t_parser *cmd, char *filename, t_redir_type type);
void ft_clean_cmd_node(t_parser *cmd); // ft_clean_init yerine daha açıklayıcı

// --- DİĞER FONKSİYON PROTOTİPLERİ ---
//oid execute_cmds(t_parser *cmd_list);

// Tokenizer fonksiyon prototipleri
t_tokenizer *tokenizer_init(char *input);
void        tokenizer_free(t_tokenizer *tokenizer);
t_token     *tokenizer_get_next_token(t_tokenizer *tokenizer);
t_token     *tokenize_input(char *input);
void        token_free(t_token *token);
void        token_list_free(t_token *tokens);
void        print_tokens(t_token *tokens);

// Yardımcı fonksiyonlar
int         is_whitespace(char c);
int         is_special_char(char c);
int         is_quote(char c);
char        *extract_complex_word(t_tokenizer *tokenizer);

t_token *tokenize_input(char *input);
t_token *token_input_2(t_tokenizer *tokenizer, t_token *tokens, t_token *current);
t_token *tokenizer_get_next_token(t_tokenizer *tokenizer);
t_token *tokenizer_get_next_token_2(t_tokenizer *tokenizer);
t_tokenizer *tokenizer_init(char *input);
t_token *create_token(t_token_type type, char *value);

void    ft_skip_space(t_tokenizer *tokenizer);
char lexer_peek(t_tokenizer *tokenizer);
int ft_ispace(char c);
void    lexer_advance(t_tokenizer *tokenizer);
void    tokenizer_free(t_tokenizer *tokenizer);
void ft_memcpy(char *s1,char *s2, int len);
int is_quote(char c);
int ft_strlen(char *s);
char *ft_strdup(char *str);


void print(t_parser *c);
void execute_cmds(t_parser *cmd_list,char **env);
void free_parser_list(t_parser *cmd_list);
int count_commands(t_parser *cmd);
int ft_strcmp(char *s1, char *s2);


//buitin
int     is_builtin(char *av);
int     built_echo(t_parser *cmd);
int     built_cd(t_parser *cmd);
int     builtin_pwd(void);
int builtin_exit(t_parser *cmd);



#endif