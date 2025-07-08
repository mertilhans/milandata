/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:09:33 by merilhan          #+#    #+#             */
/*   Updated: 2025/07/07 05:47:45 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PARSER_H
# define PARSER_H

#include "../lexer/shell.h"

typedef struct s_simple_cmd {
    char **argv;           // Komut ve argümanlar
    char *redir_in;        // < dosyası
    char *redir_out;       // > veya >> dosyası
    int append;            // 1 ise >>, 0 ise >
    struct s_simple_cmd *next; // Pipe için
} t_simple_cmd;

t_simple_cmd *parse_tokens(t_token *tokens);

#endif