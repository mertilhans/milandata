/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:09:33 by merilhan          #+#    #+#             */
/*   Updated: 2025/07/07 05:51:26 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../gb/gb.h"
#include "parser.h"
#include "../lexer/shell.h"
#include <stdio.h>

t_simple_cmd *parse_tokens(t_token *tokens)
{
    t_simple_cmd *cmd_list = NULL;
    t_simple_cmd *last_cmd = NULL;
    while (tokens && tokens->type != TOKEN_EOF)
    {
        while (tokens && (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_EOF))
            tokens = tokens->next;
        if (!tokens || tokens->type == TOKEN_EOF)
            break;
        t_simple_cmd *cmd = gb_malloc(sizeof(t_simple_cmd));
        cmd->argv = gb_malloc(sizeof(char*) * 64);
        int argc = 0;
        cmd->redir_in = NULL;
        cmd->redir_out = NULL;
        cmd->append = 0;
        cmd->next = NULL;
        while (tokens && tokens->type != TOKEN_PIPE && tokens->type != TOKEN_EOF)
        {
            if (tokens->type == TOKEN_WORD)
                cmd->argv[argc++] = tokens->value;
            else if (tokens->type == TOKEN_REDIR_IN)
            {
                tokens = tokens->next;
                if (tokens && tokens->type == TOKEN_WORD)
                    cmd->redir_in = tokens->value;
            }
            else if (tokens->type == TOKEN_REDIR_OUT)
            {
                tokens = tokens->next;
                if (tokens && tokens->type == TOKEN_WORD)
                {
                    cmd->redir_out = tokens->value;
                    cmd->append = 0;
                }
            }
            else if (tokens->type == TOKEN_REDIR_APPEND)
            {
                tokens = tokens->next;
                if (tokens && tokens->type == TOKEN_WORD)
                {
                    cmd->redir_out = tokens->value;
                    cmd->append = 1;
                }
            }
            tokens = tokens->next;
        }
        cmd->argv[argc] = NULL;
        if (argc > 0) {
            printf("Komut: ");
            for (int i = 0; cmd->argv[i]; i++)
                printf("%s ", cmd->argv[i]);
            printf("\n");
            if (cmd->redir_in)
                printf("  < input: %s\n", cmd->redir_in);
            if (cmd->redir_out)
                printf("  > output: %s (%s)\n", cmd->redir_out, cmd->append ? ">>" : ">");
            printf("--------------------------\n");
            if (!cmd_list)
                cmd_list = cmd;
            else
                last_cmd->next = cmd;
            last_cmd = cmd;
        }
        // Pipe varsa ilerle
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }
    return cmd_list;
}

/*
Algoritma Açıklaması:
1. Token listesini sırayla gez.
2. Her komut için yeni bir t_simple_cmd oluştur.
3. Komutun argümanlarını ve yönlendirmelerini (redir_in, redir_out, append) doldur.
4. Pipe (|) gelirse yeni komut başlat.
5. Komutları linked list olarak sırala.
6. Bellek işlemlerinde gb_malloc kullan, free işlemi gerekmez.
*/




