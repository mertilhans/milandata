#ifndef MINISHELL_H
# define MINISHELL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_HISTORY 50


int my_strcmp(const char *s1, const char *s2);
int is_space(char c);

#endif