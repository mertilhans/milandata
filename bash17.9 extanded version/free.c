/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 04:26:13 by husarpka          #+#    #+#             */
/*   Updated: 2025/08/02 07:33:38 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

// tek token'ı free et
void token_free(t_token *token)
{
    if (!token)
        return;
    
    if (token->value)
        gc_free(token->value);
    gc_free(token);
}

void token_list_free(t_token *tokens)
{
    t_token *current = tokens;
    t_token *next;
    while (current)
    {
        next = current->next;
        token_free(current);
        current = next;
    }

}