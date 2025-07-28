#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_heredoc {
	char *content;
    size_t len;
} t_heredoc;

typedef struct s_heredoc_buffer {
	char *content;
	char *line;
	char *line_with_nl;
	char *new_content;
	size_t delimiter_len;
} t_heredoc_buffer;



typedef struct s_reader {
    char *buf;
    size_t size;
    size_t pos;
} t_reader;




char *ft_strcpy(char *dest, const char *src)
{
    size_t i;
	
	i = 0;
    while (src[i])
	{
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

char *ft_strcat(char *dest, const char *src) 
{
    size_t i;
    size_t j;
	
	j = 0;
	i = 0;
    while (dest[i])
        i++;
    while (src[j])
	{
        dest[i + j] = src[j];
        j++;
    }
    dest[i + j] = '\0';
    return (dest);
}


typedef struct s_exec_data {
    int in_fd;
    int pipefd[2];
    int *pids;
    int i;
    int pids_size;
    char **env;
    int last_exit_status;
	int original_stdin;
    int original_stdout;
	int return_status;
} t_exec_data;

char *copy_dir(const char *dir, int len)
{
    char *res;
    int i;

	i = 0;
    res = malloc(len + 1);
    if (!res)
		return (NULL);
    while (i < len)
	{
		res[i] = dir[i];
		i++;
	}
    res[len] = '\0';
    return (res);
}

char *join_path(char *dir, char *file)
{
    size_t dlen;
    size_t flen;
	char *res;

	flen = strlen(file);
	dlen = strlen(dir);
    res = malloc(dlen + flen + 2);
    if (!res)
		return (NULL);
    ft_memcpy(res, dir, dlen);
    res[dlen] = '/';
    ft_memcpy(res + dlen + 1, file, flen);
    res[dlen + flen + 1] = '\0';
    return (res);
}

int is_exec_other(char *path)
{
    if (access(path, F_OK) != 0)
		return (0);
    if (access(path, X_OK) != 0)
		return (0);
    return (1);
}

char *check_cmd_in_dir(const char *dir_start, int len, char *cmd)
{
	char *result;
	char *dir;

    dir = copy_dir(dir_start, len);
    if (!dir)
		return (NULL);
    result = join_path(dir, cmd);
    free(dir);
    if (is_exec_other(result))
		return (result);
    free(result);
    return (NULL);
}

char *search_in_path(char *cmd, char *path)
{
    char *start;
    char *finish;
    char *found;
	int len;

	start = path;
	finish = path;
    while (1)
	{
        if (*start == ':' || *start == '\0') 
		{
            len = start - finish;
            if (len > 0)
			{
                found = check_cmd_in_dir(finish, len, cmd);
                if (found) return found;
            }
            if (*start == '\0')
				break;
            finish = start + 1;
        }
        start++;
    }
    return (NULL);
}

char *find_executable(char *cmd) 
{
	char *path;

    if (is_exec_other(cmd))
		return (ft_strdup(cmd));
    path = getenv("PATH");
    if (!path)
		return (NULL);
    return (search_in_path(cmd, path));
}

void redir_in(t_redirection *redir)
{
	int fd;

    fd = open(redir->filename, O_RDONLY);
    if (fd < 0)
	{
        perror("open <");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redir_out(t_redirection *redir)
{
	int fd;

    fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd < 0)
	{
        perror("open >");
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void redir_append(t_redirection *redir)
{
	int fd;

    fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (fd < 0)
	{
        perror("open >>");
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}


char *expand_buf(t_reader *r, size_t new_size)
{
    char *new_buf;
	size_t i;

	new_buf = malloc(new_size);
    if (!new_buf)
        return NULL;
    i = 0;
    while (i < r->pos) 
	{
        new_buf[i] = r->buf[i];
        i++;
    }
    free(r->buf);
    r->buf = new_buf;
    r->size = new_size;
    return (r->buf);
}

int add_char(t_reader *r, char c)
{
	size_t new_size;

    if (r->pos + 1 >= r->size)
	{
        new_size = r->size * 2;
        if (new_size < r->size + 1)
        	new_size = r->size + 1;
        if (!expand_buf(r, new_size))
            return (0);
    }
    r->buf[r->pos++] = c;
    return (1);
}

char *ft_readline_loop(t_reader *r)
{
    char c;
    ssize_t n;
    while ((n = read(STDIN_FILENO, &c, 1)) > 0)
	{
        if (c == '\n')
            break;
        if (!add_char(r, c))
		{
            free(r->buf);
            return NULL;
        }
    }
    if (n <= 0 && r->pos == 0)
	{
        free(r->buf);
        return NULL;
    }
    if (r->pos + 1 >= r->size && !expand_buf(r, r->pos + 1))
        return NULL;
    r->buf[r->pos] = '\0';
    return (r->buf);
}

char *ft_readline(const char *prompt)
{
    t_reader r;
    r.size = 1;
    r.pos = 0;
    r.buf = malloc(r.size);
    if (!r.buf)
        return NULL;
    if (prompt)
        write(STDOUT_FILENO, prompt, strlen(prompt));
    return ft_readline_loop(&r);
}


int heredoc_append_line(t_heredoc_buffer *buf)
{
    size_t line_len;
	
	line_len = strlen(buf->line);
    buf->line_with_nl = malloc(line_len + 2);
    if (!buf->line_with_nl)
        return 0;
    strcpy(buf->line_with_nl, buf->line);
    buf->line_with_nl[line_len] = '\n';
    buf->line_with_nl[line_len + 1] = '\0';
    buf->new_content = malloc(strlen(buf->content) + strlen(buf->line_with_nl) + 1);
    if (!buf->new_content) 
	{
        free(buf->line_with_nl);
        return 0;
    }
    ft_strcpy(buf->new_content, buf->content);
    ft_strcat(buf->new_content, buf->line_with_nl);
    free(buf->content);
    free(buf->line_with_nl);
    buf->content = buf->new_content;
    return (1);
}

char *readline_loop(t_heredoc_buffer *buf,const char *delimiter)
{
	while (1) 
	{
		buf->line = ft_readline("> ");
		if (!buf->line)
			break;
		if ((size_t)ft_strlen(buf->line) == buf->delimiter_len && 
			ft_strcmp(buf->line, delimiter) == 0)
		{
			free(buf->line);
			break;
		}
		if (!heredoc_append_line(buf)) 
		{
			perror("Satır eklenemedi");
			free(buf->line);
			free(buf->content);
			return NULL;
		}
		free(buf->line);
		buf->line = NULL;
	}
	return (NULL);
}
char *read_single_heredoc_block(const char *delimiter)
{
	t_heredoc_buffer buf;
    if (!delimiter || *delimiter == '\0') 
	{
        fprintf(stderr, "Hata: Heredoc ayirici kelimesi belirtilmedi.\n");
        return NULL;
    }
    buf.content = strdup("");
    buf.line = NULL;
    buf.line_with_nl = NULL;
    buf.new_content = NULL;
    buf.delimiter_len = strlen(delimiter);
    if (!buf.content)
	{
        perror("Heredoc bellek ayirma basarisiz");
        return NULL;
    }
	readline_loop(&buf,delimiter);
    return buf.content;
}


int ft_h_built(t_redirection *current_redir,char *heredoc_content,int *last_heredoc_fd, size_t *heredoc_len)
{
	int pipefd[2];
	heredoc_content = read_single_heredoc_block(current_redir->filename);
	if (!heredoc_content)
	{
		if (*last_heredoc_fd != -2 && *last_heredoc_fd != -1) close(*last_heredoc_fd);
		return -1;
	}
	*heredoc_len = strlen(heredoc_content);
	if (pipe(pipefd) == -1)
	{
		perror("heredoc pipe");
		free(heredoc_content); 
		if (*last_heredoc_fd != -2 && *last_heredoc_fd != -1) close(*last_heredoc_fd);
			return (-1);
	}
	write(pipefd[1], heredoc_content, *heredoc_len);
	close(pipefd[1]); 
	free(heredoc_content); 
	heredoc_content = NULL; 
	*last_heredoc_fd = pipefd[0]; 
	return (0);
}

int h_loop(t_parser *cmd,char *heredoc_content,int *last_heredoc_fd,size_t *heredoc_len)
{
	t_redirection *current_redir;

	current_redir = cmd->redirs;
	while (current_redir) 
	{
		if (current_redir->type == REDIR_HEREDOC)
		{
			if (heredoc_content)
			{
				free(heredoc_content);
				heredoc_content = NULL;
			}
			if (*last_heredoc_fd != -2 && *last_heredoc_fd != -1)
			{
				close(*last_heredoc_fd);
				*last_heredoc_fd = -1; 
			}
			if(ft_h_built(current_redir,heredoc_content,last_heredoc_fd,heredoc_len) == -1)
				return (-1);
		}
		current_redir = current_redir->next;
	}
return (0);
}

int process_heredocs(t_parser *cmd)
{
    int last_heredoc_fd;
    size_t heredoc_len;
    char *heredoc_content;

	heredoc_content = NULL;
	heredoc_len = 0;
	last_heredoc_fd = -2;
	if(h_loop(cmd,heredoc_content,&last_heredoc_fd,&heredoc_len) == -1)
		return (-1);
    return last_heredoc_fd;
}

void setup_file_redirections(t_parser *cmd)
{
    t_redirection *current_redir;

    current_redir = cmd->redirs;
    while (current_redir)
	{
        if (current_redir->type == REDIR_IN)
            redir_in(current_redir);
        else if (current_redir->type == REDIR_OUT)
            redir_out(current_redir);
        else if (current_redir->type == REDIR_APPEND)
            redir_append(current_redir);
        current_redir = current_redir->next;
    }
}


void built_echo(t_parser *cmd)
{
    int i;
	
	i = 1;
    while (cmd->argv[i])
	{
        printf("%s", cmd->argv[i]);
        if (cmd->argv[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
}


void built_cd(t_parser *cmd)
{
    if (cmd->argv[1]) 
	{
        if (chdir(cmd->argv[1]) != 0)
        	perror("cd");
    } 
    
}

void builtin_pwd()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else 
        perror("pwd");
}

void builtin_exit(t_parser *cmd)
{
    int exit_code;
	
	exit_code = 0;
    if (cmd->argv[1])
        exit_code = atoi(cmd->argv[1]); // atoi yaz unutmaaaaa.
    exit(exit_code);
}

int ft_builtin_call(t_parser *cmd, t_exec_data *data)
{
    if (ft_strcmp(cmd->argv[0], "echo") == 0)
	{
        built_echo(cmd);
        data->last_exit_status = 0;
        return (0);
    }
    if (ft_strcmp(cmd->argv[0], "cd") == 0)
	{
        built_cd(cmd);
        data->last_exit_status = 0; 
        return (0);
    }
    if (ft_strcmp(cmd->argv[0], "pwd") == 0)
	{
        builtin_pwd();
        data->last_exit_status = 0;
        return (0);
    }
    if (ft_strcmp(cmd->argv[0], "exit") == 0)
	{
        builtin_exit(cmd);
        return (0);
    }
    return 1;
}

int is_builtin(t_parser *cmd)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (0);
    if (strcmp(cmd->argv[0], "exit") == 0 || strcmp(cmd->argv[0], "cd") == 0)
        return (1);
    if (strcmp(cmd->argv[0], "pwd") == 0 || strcmp(cmd->argv[0], "echo") == 0)
        return (1);
    return 0;
}

int count_commands(t_parser *cmd)
{
	int count;

    count = 0;
    while (cmd)
	{
		count++;
        cmd = cmd->next;
    }
    return (count);
}
void ft_redir_ctrl(t_parser *cmd)
{
	t_redirection *redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			redir_in(redir);
		else if (redir->type == REDIR_OUT)
			redir_out(redir);
		else if (redir->type == REDIR_APPEND)
			redir_append(redir);
		else if (redir->type == REDIR_HEREDOC) 
		{
			if (cmd->heredoc_fd != -1 && cmd->heredoc_fd != -2)
			{
				if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 cmd->heredoc_fd");
					exit(1);
				}
				close(cmd->heredoc_fd); 
				cmd->heredoc_fd = -1; 
			}
		}
		redir = redir->next;
	}
}
void ft_exec_start(t_parser *cmd,t_exec_data *data)
{
	char *exec_path;
	if (is_builtin(cmd))
	{
		ft_builtin_call(cmd, data);
		exit(data->last_exit_status);
	} 
	else 
	{
		exec_path = find_executable(cmd->argv[0]);
		if (!exec_path)
		{
			fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
			exit(127);
		}
		execve(exec_path, cmd->argv, data->env);
		perror("execve");
		exit(1);
	}

}
void child_process_exec(t_parser *cmd, t_exec_data *data)
{

    if (data->in_fd != STDIN_FILENO)
	{
        if (dup2(data->in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			exit(1);
		}
        close(data->in_fd);
    }
    if (cmd->next)
	{
        if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 pipefd[1]");
			exit(1);
		}
        close(data->pipefd[0]);
        close(data->pipefd[1]);
    }
	ft_redir_ctrl(cmd);
	ft_exec_start(cmd,data);
}

int setup_pipe_if_needed(t_parser *cmd, t_exec_data *data)
{
    if (!cmd->next)
        return (0);
    if (pipe(data->pipefd) == -1) 
	{
        perror("pipe");
        return -1;
    }
    return 0;
}

int fork_pipe_exec(t_parser *cmd, t_exec_data *data)
{
    int pid;

    pid = fork();
    if (pid == -1) 
	{
        perror("fork");
        return -1;
    }
    if (pid == 0) 
	{ 
        child_process_exec(cmd, data);
        exit(1);
    }
    return (pid);
}

int setup_and_fork(t_parser *cmd, t_exec_data *data)
{
    int pid;

    if (setup_pipe_if_needed(cmd, data) == -1) 
        return (-1);
    pid = fork_pipe_exec(cmd, data);
    if (pid == -1) 
	{
        if (cmd->next) 
		{
            close(data->pipefd[0]);
            close(data->pipefd[1]);
        }
        return -1;
    }

    if (data->i >= data->pids_size) 
	{
        printf("limit error\n");
        return (-1);
    }
    data->pids[data->i++] = pid;
    return (pid);
}

int process_command(t_parser *cmd, t_exec_data *data)
{
    if (!cmd->argv || !cmd->argv[0])
        return 0;
    if (setup_and_fork(cmd, data) == -1)
        return 0;
    if (data->in_fd != STDIN_FILENO)
        close(data->in_fd);
    if (cmd->next)
	{
        close(data->pipefd[1]);
        data->in_fd = data->pipefd[0];
    } 
	else
        data->in_fd = STDIN_FILENO;
    return 1;
}

void execute_loop(t_parser *cmd_list, t_exec_data *data)
{
    data->in_fd = STDIN_FILENO;
    t_parser *cmd;

    cmd = cmd_list;
    while (cmd)
	{
        if (!process_command(cmd, data))
		{
            break;
        }
        cmd = cmd->next;
    }
    if (data->in_fd != STDIN_FILENO)
        close(data->in_fd);
}

void wait_pids(t_exec_data *data)
{
    int status;
    int j;
	
	j = 0;
    while (j < data->i) 
	{
        waitpid(data->pids[j], &status, 0);
        if (j == data->i - 1)
		{
            if (WIFEXITED(status))
                data->last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                data->last_exit_status = 128 + WTERMSIG(status);
        }
        j++;
    }
}


void data_handle(t_exec_data *data, t_parser *cmd_list,int *pid_len, char **env) 
{
	*pid_len = count_commands(cmd_list);
	data->pids_size = *pid_len;
    data->pids = NULL;
    data->i = 0;
    data->env = env;
    data->in_fd = STDIN_FILENO;
    data->last_exit_status = 0;
}

void free_parser_list(t_parser *cmd_list) {
    t_parser *current_cmd;
    t_parser *next_cmd;
    t_redirection *current_redir;
    t_redirection *next_redir;

    current_cmd = cmd_list;
    while (current_cmd) {
       
        if (current_cmd->argv) {
            for (int i = 0; current_cmd->argv[i]; i++) {
                free(current_cmd->argv[i]); 
            }
            free(current_cmd->argv);
        }

   
        current_redir = current_cmd->redirs;
        while (current_redir) {
            next_redir = current_redir->next;
            free(current_redir->filename);
            free(current_redir);
            current_redir = next_redir;
        }

        next_cmd = current_cmd->next;
        free(current_cmd);
        current_cmd = next_cmd;
    }
}


int heredoc_handle(t_parser *cmd_list,t_exec_data *data)
{
	t_parser *current_cmd;
	current_cmd = cmd_list;
	while (current_cmd) 
	{
		current_cmd->heredoc_fd = process_heredocs(current_cmd);
		if (current_cmd->heredoc_fd == -1) 
		{
			dup2(data->original_stdin, STDIN_FILENO);
			close(data->original_stdin);
			dup2(data->original_stdout, STDOUT_FILENO);
			close(data->original_stdout);
			t_parser *tmp_cmd = cmd_list;
			while(tmp_cmd != current_cmd->next) 
			{
				if (tmp_cmd && tmp_cmd->heredoc_fd != -1 && tmp_cmd->heredoc_fd != -2) 
				{
					close(tmp_cmd->heredoc_fd);
					tmp_cmd->heredoc_fd = -1;
				}
				tmp_cmd = tmp_cmd->next;
			}
			return 1;
		}
		current_cmd = current_cmd->next;
	}
	return (0); 
}
int n_next_or_built(t_parser *cmd_list, t_exec_data *data)
{
	if (cmd_list->heredoc_fd != -1 && cmd_list->heredoc_fd != -2) 
	{
		if (dup2(cmd_list->heredoc_fd, STDIN_FILENO) == -1) 
		{
			perror("dup2 heredoc for builtin");
			data->return_status = 1;
		}
		close(cmd_list->heredoc_fd);
		cmd_list->heredoc_fd = -1;
	}
	if (data->return_status == 0) 
	{
		setup_file_redirections(cmd_list);
		ft_builtin_call(cmd_list, data);
		
	}
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
	return (data->last_exit_status);
	
}
int execute_and_pids(t_parser *cmd_list,t_exec_data *data)
{
	execute_loop(cmd_list, data);
	wait_pids(data);
	free(data->pids);
	return (0);
}
int ft_data_pids(t_parser *cmd_list, t_exec_data *data)
{
	t_parser *current_cmd;

	current_cmd = cmd_list;
	data->pids = malloc(sizeof(int) * data->pids_size);
	if (!data->pids) 
	{
		perror("pids error\n");
		current_cmd = cmd_list;
		while(current_cmd) 
		{
			if (current_cmd->heredoc_fd != -1 && current_cmd->heredoc_fd != -2) 
			{
				close(current_cmd->heredoc_fd);
				current_cmd->heredoc_fd = -1;
			}
			current_cmd = current_cmd->next;
		}
		dup2(data->original_stdin, STDIN_FILENO);
		close(data->original_stdin);
		dup2(data->original_stdout, STDOUT_FILENO);
		close(data->original_stdout);
		return (1);
	}
	execute_and_pids(cmd_list,data);
	return (0);
}
void finish_fd(t_parser *cmd_list,t_exec_data *data)
{
	t_parser *current_cmd;

	current_cmd = cmd_list;
	while(current_cmd) 
	{
		if (current_cmd->heredoc_fd != -1 && current_cmd->heredoc_fd != -2) {
			close(current_cmd->heredoc_fd);
			current_cmd->heredoc_fd = -1;
		}
		current_cmd = current_cmd->next;
	}
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
	
}
int execute_cmds(t_parser *cmd_list, char **env)
{
    t_exec_data data;
	int pid_len;
    data.return_status = 0;
	data.original_stdin = dup(STDIN_FILENO);
	data.original_stdout = dup(STDOUT_FILENO);
    if (data.original_stdin == -1 || data.original_stdout == -1) 
	{
		perror("dup failed");
        return 1;
    }
	if (heredoc_handle(cmd_list,&data))
		return (1);
    data_handle(&data, cmd_list,&pid_len, env);

    if (pid_len == 1 && is_builtin(cmd_list)) 
	{
		if(n_next_or_built(cmd_list,&data))
			return (data.return_status);
		return(0);
    }
	if (ft_data_pids(cmd_list,&data))
		return (1);
	finish_fd(cmd_list,&data);
	return (data.last_exit_status);
}
