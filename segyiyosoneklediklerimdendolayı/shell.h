#ifndef SHELL_H
# define SHELL_H

// Global environment list pointer
struct s_env;
extern struct s_env *g_env_list;

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <ctype.h>
# include <errno.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

struct s_env;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
	TOKEN_ERROR
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection
{
	t_redir_type			type;
	char					*filename;
	int						no_expand;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_parser
{
	char			**argv;
	int				argv_cap;
	t_redirection	*redirs;
	int				parse_error;
	int				heredoc_fd;
	struct s_parser	*next;
	int				fd_i;
	int				fd_o;
}	t_parser;

typedef struct s_all
{
	t_parser		*cmd_list;
	t_parser		*last_cmd;
	struct s_env	*env_list;
}	t_all;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_export
{
	char			*key;
	char			*value;
	struct s_export	*next;
}	t_export;

typedef struct s_builder
{
	char	*str;
	int		len;
	int		cap;
}	t_builder;

typedef struct s_exec_data
{
	int		in_fd;
	int		pipefd[2];
	int		*pids;
	int		i;
	int		pids_size;
	char	**env;
	int		original_stdin;
	int		original_stdout;
}	t_exec_data;

typedef struct s_gb
{
	void		*data;
	struct s_gb	*next;
}	t_gb;

typedef struct s_tokenizer
{
	char	*input;
	int		pos;
	int		len;
	char	current;
}	t_tokenizer;

typedef struct s_heredoc_data {
	int last_heredoc_fd;
	size_t heredoc_len;
	char *heredoc_content;
} t_heredoc_data;

typedef struct s_heredoc_buffer {
	char *content;
	char *line;
	char *line_with_nl;
	char *new_content;
	size_t delimiter_len;
} t_heredoc_buffer;

typedef struct s_token_data {
	char *word;
	char in_quote;
	int len;
	int capacity;
} t_token_data;

void		shell_loop(t_env *env_list, char **env);
int			process_command_line(char *line, t_env **env_list, char **env);
char		*find_executable(char *cmd);
int			built_cd(t_parser *cmd);
void		built_echo(t_parser *cmd);
void		builtin_pwd(void);
void		builtin_env(t_env *env_list);
void		builtin_export(t_parser *cmd, t_env **env_list);
void		builtin_unset(t_parser *cmd, t_env **env_list);
void		builtin_exit(t_parser *cmd);
int			is_numeric_string(char *str);
int			execute_cmds(t_parser *cmd_list, char **env, t_env **env_list);
void		execute_loop(t_parser *cmd_list, t_exec_data *data, t_env **env_list);
void		child_process_exec(t_parser *cmd, t_exec_data *data, t_env **env_list);
void		ft_exec_start(t_parser *cmd, t_exec_data *data, t_env **env_list);
int			is_builtin(t_parser *cmd);
int			ft_builtin_call(t_parser *cmd, t_exec_data *data, t_env **env_list);
int			n_next_or_built(t_parser *cmd_list, t_exec_data *data, t_env **env_list);
int			ft_redir_ctrl(t_parser *cmd);
int         setup_file_redirections(t_parser *cmd);
int			count_commands(t_parser *cmd);
int			data_init(t_exec_data *data, t_parser *cmd_list, char **env);
int			ft_data_pids(t_parser *cmd_list, t_exec_data *data, t_env **env_list);
void		ft_putendl_fd(char *str, int fd);
void		close_all_fds_except_std(t_parser *cmd);
void		close_fd(t_exec_data *data);
void		wait_pids(t_exec_data *data);
void		finish_fd(t_parser *cmd_list, t_exec_data *data);
t_env		*init_env(char **env);
void		set_env_value(t_env **env_list, char *key, char *value);
void		unset_env_value(t_env **env_list, char *key);
t_env		*find_env(t_env *env_list, const char *key);
char		*get_env_value(t_env *env_list, char *key);
char        *find_env_value(t_env *env_list, char *key);
char		**env_list_to_array(t_env *env_list);
void		free_env_list(t_env *env_list);
t_export	**get_export_list(void);
t_export	*find_export(t_export *export_list, const char *key);
char		*find_export_value(char *key);
void		set_export_value(t_export **export_list, const char *key, const char *value);
void		unset_export_value(t_export **export_list, const char *key);
void		free_export_list(t_export *export_list);
t_export	*init_export_from_env(t_env *env_list);
void		export_process_keyvalue(t_parser *cmd, int *i, t_env **env_list);
void		export_print_all(t_export **export_list);
char		*export_build_value(t_parser *cmd, int *i, char *value);
int			is_valid_identifier(const char *identifier);
t_token		*tokenize_input(char *input);
t_token     *tokenizer_get_next_token(t_tokenizer *tokenizer);
void		tokenizer_free(t_tokenizer *tokenizer);
void		lexer_advance(t_tokenizer *tokenizer);
void		ft_skip_space(t_tokenizer *tokenizer);
int         ft_ispace(char c);
char		*extract_complex_word(t_tokenizer *tokenizer);
t_tokenizer	*tokenizer_init(char *input);
t_token		*create_token(t_token_type type, char *value);
void		cleanup_env_array(char **env_array);
t_parser	*parse_tokens(t_token *tokens, t_env *env_list);
t_token		*ft_control_token(t_token *tokens, t_all *data);
void		ft_loop(t_token **tokens, t_parser *cmd, int *argc, t_all *all);
void		add_redirection(t_parser *cmd, t_redir_type type, char *filename);
void		ft_redir_heredoc(t_token *tokens, t_parser *cmd);
void		expand_argv_capacity(t_parser *cmd, int *argc);
void		cleanup_split_args(char **split_args);
void		add_split_args(t_parser *cmd, int *argc, char **split_args);
void		handle_split_expansion(t_parser *cmd, int *argc, char *o, char *e);
void		handle_normal_expansion(t_parser *cmd, int *argc, t_token *t, char *e);
char	    **split_expanded_string(char *str);
int			has_quote_chars(char *delimiter);
char		*remove_quotes(char *delimiter);
int			token_ctrl(t_token *tokens);
int			ft_syn(t_token *tokens);
int			should_split_expansion(char *original_token, char *expanded);
int         is_empty_expansion(char *original, char *expanded);
char		*expand_with_quotes(char *str, t_env *env_list);
char		*expand_heredoc_line(char *str, t_env *env_list);
int			is_valid_char(char c);
char		*handle_dollar(char *str, int *i, t_env *env_list);
void		builder_init(t_builder *builder, int initial_cap);
int			builder_push_char(t_builder *builder, char c);
int			builder_append_str(t_builder *builder, char *to_add);
char		*expand_variables(char *str, t_env *env_list);
char		*handle_simple(const char *str, int *i, t_env *env_list);
int			heredoc_handle(t_parser *cmd_list, t_exec_data *data, t_env *env_list);
int			heredoc_fd_error(t_parser *cmd_list, t_exec_data *data, t_parser *current_cmd);
char		*heredoc_readline(const char *prompt);
char		*read_single_heredoc_block(char *delimiter);
char		*read_heredoc_with_expand(char *delimiter, t_env *env_list);
char		*readline_loop(t_heredoc_buffer *buf, const char *delimiter);
char		*readline_loop_expand(t_heredoc_buffer *buf, const char *del, t_env *env);
int			heredoc_append_line(t_heredoc_buffer *buf);
int			heredoc_append_expanded(t_heredoc_buffer *buf, t_env *env_list);
int			ft_h_built_expand(t_redirection *redir, t_heredoc_data *data, t_env *e_list);
void		setup_interactive_signals(void);
void		setup_parent_execution_signals(void);
void		setup_child_signals(void);
void        handle_sigint_heredoc(int signum);
void        handle_sigint(int signum);
int			calculate_exit_status(int status);
void		heredoc_signals(void);
void		init_shell_variables(t_env **env_list);
int			get_last_exit_status(void);
void		set_last_exit_status(int status);
void		*gb_malloc(size_t size);
void		gb_free_all(void);
void		gc_free_from_list(t_gb **gb_list, void *ptr);
void		gc_free(void *ptr);
t_gb		**get_gb_list(void);
t_gb		*ft_lstnew(void *data);
void		gb_lstadd_front(t_gb **lst, t_gb *new_node);
void		fail_exit(void);
void		*gc_safe(void *ptr);
t_gb		**get_env_gb_list(void);
void		*env_gc_safe(void *ptr);
void		*env_gb_malloc(size_t size);
void		env_gb_free_all(void);
void		env_gc_free(void *ptr);
int			ft_atoi(char *str);
void		ft_memcpy(char *s1, const char *s2, int len);
int			is_quote(char c);
int			ft_strlen(char *s);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strdup(char *str);
char		*ft_strndup(const char *str, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strncpy(char *dest, const char *src, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_itoa(int n);
char		*ft_strcpy(char *dest, const char *src);
char		*ft_strcat(char *dest, const char *src);
t_env       **get_env_list(void);
char lexer_peek(t_tokenizer *tokenizer);

#endif